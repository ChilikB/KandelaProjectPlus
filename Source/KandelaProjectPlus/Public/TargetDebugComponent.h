// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetDebugComponent.generated.h"

UENUM(BlueprintType)
enum class ETargetSide : uint8
{
    Center UMETA(DisplayName = "Center"),
    Left   UMETA(DisplayName = "Left"),
    Right  UMETA(DisplayName = "Right"),
    Behind UMETA(DisplayName = "Behind"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KANDELAPROJECTPLUS_API UTargetDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetDebugComponent();
    virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    AActor* TargetActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    bool bFlattenZ = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    float SideEpsilon = 0.01f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    float BehindDotThreshold = -0.1f;

    // Debug
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    bool bDrawDebug = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    float DebugForwardLen = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    float DebugDirLen = 250.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target Debug")
    FVector DirectionToTarget = FVector::ZeroVector;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target Debug")
    float AngleToTargetDeg = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target Debug")
    ETargetSide Side = ETargetSide::Center;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Debug")
    FName TargetTag = "TestTarget";
private:
    bool ComputeTargetInfo(const AActor* OwnerActor, const AActor* Target, FVector& OutDir, float& OutAngleDeg, ETargetSide& OutSide) const;
};
