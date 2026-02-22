// Fill out your copyright notice in the Description page of Project Settings.

#include "HealingPickup.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameplayEffect.h"
#include "CharacterAttributeSet.h"
#include "GameplayTagContainer.h"

AHealingPickup::AHealingPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(RootComponent);

    Sphere->SetSphereRadius(120.f);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AHealingPickup::BeginPlay()
{
    Super::BeginPlay();
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHealingPickup::OnSphereBeginOverlap);
}

void AHealingPickup::OnSphereBeginOverlap(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32,
    bool,
    const FHitResult&)
{
    if (!HasAuthority()) return;
    if (!OtherActor || !HealEffectClass) return;

    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
    if (!ASC) return;

    const UCharacterAttributeSet* AttrSet = ASC->GetSet<UCharacterAttributeSet>();
    if (!AttrSet) return;

    const float Health = AttrSet->GetHealth();
    const float MaxHealth = AttrSet->GetMaxHealth();

    if (Health >= MaxHealth)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealingPickup: HP full (%f/%f). Not consumed."), Health, MaxHealth);
        return;
    }

    FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
    Ctx.AddSourceObject(this);

    FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(HealEffectClass, 1.f, Ctx);
    if (!Spec.IsValid()) return;

    Spec.Data->SetSetByCallerMagnitude(
        FGameplayTag::RequestGameplayTag(FName("Data.Heal")),
        HealAmount
    );

    ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

    UE_LOG(LogTemp, Warning, TEXT("HealingPickup: Healed and consumed. (%f/%f)"), Health, MaxHealth);

    Destroy();
}