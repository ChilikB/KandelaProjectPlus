// Fill out your copyright notice in the Description page of Project Settings.


#include "GEStaminaDrain.h"
#include "CharacterAttributeSet.h"

UGEStaminaDrain::UGEStaminaDrain() {
    DurationPolicy = EGameplayEffectDurationType::Infinite;
    Period.Value = 0.1f;

    FGameplayModifierInfo Mod;
    Mod.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
    Mod.ModifierOp = EGameplayModOp::Additive;
    
    FScalableFloat Mag(-5.0f);
    Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(Mag);

    Modifiers.Add(Mod);
}