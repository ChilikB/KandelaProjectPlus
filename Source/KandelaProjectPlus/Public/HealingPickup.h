// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealingPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UGameplayEffect;

UCLASS()
class KANDELAPROJECTPLUS_API AHealingPickup : public AActor
{
    GENERATED_BODY()

public:
    AHealingPickup();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USphereComponent> Sphere;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
    TSubclassOf<UGameplayEffect> HealEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    float HealAmount = 25.f;

    UFUNCTION()
    void OnSphereBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};