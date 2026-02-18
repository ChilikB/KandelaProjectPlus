#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASprint.generated.h"

class UGameplayEffect;

UCLASS()
class KANDELAPROJECTPLUS_API UGASprint : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGASprint();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Sprint")
    float SprintSpeed = 900.f;

    UPROPERTY(EditDefaultsOnly, Category = "Sprint")
    float MinStaminaToStart = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    FGameplayTag SprintTag;

private:
    float SavedWalkSpeed = 0.f;
    FActiveGameplayEffectHandle DrainHandle;

    UFUNCTION()
    void OnStaminaChanged();
};