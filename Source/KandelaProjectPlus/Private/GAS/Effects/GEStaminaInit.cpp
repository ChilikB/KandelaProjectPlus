// Fill out your copyright notice in the Description page of Project Settings.


#include "GEStaminaInit.h"
#include "CharacterAttributeSet.h"

UGEStaminaInit::UGEStaminaInit() {

    DurationPolicy = EGameplayEffectDurationType::Instant;

    // MaxStamina = 100
    {
        FGameplayModifierInfo Mod;
        Mod.Attribute = UCharacterAttributeSet::GetMaxStaminaAttribute();
        Mod.ModifierOp = EGameplayModOp::Override;

        FScalableFloat Mag(100.f);
        Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(Mag);

        Modifiers.Add(Mod);
    }

    // Stamina = 100
    {
        FGameplayModifierInfo Mod;
        Mod.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
        Mod.ModifierOp = EGameplayModOp::Override;

        FScalableFloat Mag(100.f);
        Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(Mag);

        Modifiers.Add(Mod);
    }
}