#include "GASprint.h"

#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GEStaminaDrain.h"

UGASprint::UGASprint()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    SprintTag = FGameplayTag::RequestGameplayTag(FName("Ability.Sprint"), false);

    if (SprintTag.IsValid())
    {
        AbilityTags.AddTag(SprintTag);
    }
}

void UGASprint::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
    if (!ASC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    const UCharacterAttributeSet* AS =
        Cast<UCharacterAttributeSet>(ASC->GetAttributeSet(UCharacterAttributeSet::StaticClass()));

    if (!AS || AS->GetStamina() < MinStaminaToStart)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    UCharacterMovementComponent* Move = Character->GetCharacterMovement();
    SavedWalkSpeed = Move->MaxWalkSpeed;
    Move->MaxWalkSpeed = SprintSpeed;

    const FGameplayEffectSpecHandle Spec =
        ASC->MakeOutgoingSpec(UGEStaminaDrain::StaticClass(), 1.f, ASC->MakeEffectContext());

    if (Spec.IsValid())
    {
        DrainHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
    }
}

void UGASprint::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
    {
        if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
        {
            Move->MaxWalkSpeed = SavedWalkSpeed;
        }
    }

    if (ActorInfo->AbilitySystemComponent.IsValid() && DrainHandle.IsValid())
    {
        ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(DrainHandle);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
