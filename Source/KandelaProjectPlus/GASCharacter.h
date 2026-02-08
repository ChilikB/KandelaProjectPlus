// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KandelaProjectPlusCharacter.h"
#include "AbilitySystemInterface.h"
#include "GASCharacter.generated.h"

class UAbilitySystemComponent;
class UCharacterAttributeSet;

UCLASS()
class KANDELAPROJECTPLUS_API AGASCharacter : public AKandelaProjectPlusCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	AGASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCharacterAttributeSet> AttributeSet;
};
