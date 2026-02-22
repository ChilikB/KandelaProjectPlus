// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageZone.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "CharacterAttributeSet.h"
#include "TimerManager.h"

ADamageZone::ADamageZone()
{
    PrimaryActorTick.bCanEverTick = false;

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SetRootComponent(Sphere);

    Sphere->InitSphereRadius(250.f);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Sphere->SetCollisionObjectType(ECC_WorldDynamic);
    Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Sphere->SetGenerateOverlapEvents(true);
}

void ADamageZone::BeginPlay()
{
    Super::BeginPlay();

    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnBeginOverlap);
    Sphere->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnEndOverlap);

    UE_LOG(LogTemp, Warning, TEXT("DamageZone BeginPlay: %s"), *GetName());
}

void ADamageZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(DamageTimer);
    CurrentTarget = nullptr;

    Super::EndPlay(EndPlayReason);
}

void ADamageZone::OnBeginOverlap(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32,
    bool,
    const FHitResult&)
{
    if (!HasAuthority()) return;
    if (!OtherActor || !DamageEffectClass) return;

    if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
        return;

    CurrentTarget = OtherActor;

    UE_LOG(LogTemp, Warning, TEXT("DamageZone: ENTER %s"), *GetNameSafe(OtherActor));

    DealDamageTick();

    GetWorldTimerManager().SetTimer(
        DamageTimer,
        this,
        &ADamageZone::DealDamageTick,
        TickInterval,
        true
    );
}

void ADamageZone::OnEndOverlap(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32)
{
    if (!HasAuthority()) return;

    if (OtherActor && OtherActor == CurrentTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("DamageZone: EXIT %s"), *GetNameSafe(OtherActor));

        CurrentTarget = nullptr;
        GetWorldTimerManager().ClearTimer(DamageTimer);
    }
}

void ADamageZone::DealDamageTick()
{
    if (!HasAuthority()) return;
    if (!CurrentTarget || !DamageEffectClass) return;

    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurrentTarget);
    if (!ASC) return;

    FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
    Ctx.AddSourceObject(this);

    FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(DamageEffectClass, 1.f, Ctx);
    if (!Spec.IsValid()) return;

    Spec.Data->SetSetByCallerMagnitude(
        FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
        -DamagePerTick
    );
    const float HP_Before = ASC->GetNumericAttribute(UCharacterAttributeSet::GetHealthAttribute());
    UE_LOG(LogTemp, Warning, TEXT("DamageZone: HP BEFORE = %f"), HP_Before);

    ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

    const float HP_After = ASC->GetNumericAttribute(UCharacterAttributeSet::GetHealthAttribute());
    UE_LOG(LogTemp, Warning, TEXT("DamageZone: HP AFTER  = %f"), HP_After);

    UE_LOG(LogTemp, Warning, TEXT("DamageZone: TICK damage=%f to %s"), DamagePerTick, *GetNameSafe(CurrentTarget));
}

