// Fill out your copyright notice in the Description page of Project Settings.


#include "GEStaminaRegen.h"
#include "CharacterAttributeSet.h"

UGEStaminaRegen::UGEStaminaRegen() {
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    Period.Value = 0.1f;

    FGameplayModifierInfo Mod;
    Mod.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
    Mod.ModifierOp = EGameplayModOp::Additive;

    FScalableFloat Mag(1.5f);
    Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(Mag);

    Modifiers.Add(Mod);
}