// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacter.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.h"

AGASCharacter::AGASCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
