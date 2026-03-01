// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDebugComponent.h"

#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTargetDebugComponent::UTargetDebugComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTargetDebugComponent::BeginPlay()
{
    Super::BeginPlay();

    if (TargetActor) return;

    TArray<AActor*> Tagged;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, Tagged);
    if (Tagged.Num() > 0)
    {
        TargetActor = Tagged[0];
        return;
    }

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Found);
    if (Found.Num() > 0)
    {
        TargetActor = Found[0];
    }
}

void UTargetDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const AActor* OwnerActor = GetOwner();
    if (!OwnerActor || !TargetActor) return;

    FVector Dir;
    float Angle;
    ETargetSide OutSide;

    if (!ComputeTargetInfo(OwnerActor, TargetActor, Dir, Angle, OutSide)) return;

    DirectionToTarget = Dir;
    AngleToTargetDeg = Angle;
    Side = OutSide;

    if (bDrawDebug && GetWorld())
    {
        const FVector OwnerPos = OwnerActor->GetActorLocation();
        const FVector Forward = OwnerActor->GetActorForwardVector().GetSafeNormal();
        const FVector TargetPos = TargetActor->GetActorLocation();

        DrawDebugLine(GetWorld(), OwnerPos, OwnerPos + Forward * DebugForwardLen, FColor::Green, false, 0.f, 0, 2.f);

        DrawDebugLine(GetWorld(), OwnerPos, OwnerPos + DirectionToTarget * DebugDirLen, FColor::Yellow, false, 0.f, 0, 2.f);

        DrawDebugLine(GetWorld(), OwnerPos, TargetPos, FColor::Cyan, false, 0.f, 0, 1.f);

        const TCHAR* SideStr =
            (Side == ETargetSide::Left) ? TEXT("Left") :
            (Side == ETargetSide::Right) ? TEXT("Right") :
            (Side == ETargetSide::Behind) ? TEXT("Behind") :
            TEXT("Center");

        DrawDebugString(GetWorld(), OwnerPos + FVector(0, 0, 100.f),
            FString::Printf(TEXT("Angle: %.1f deg | Side: %s"), AngleToTargetDeg, SideStr),
            nullptr, FColor::White, 0.f, true);
    }
}

bool UTargetDebugComponent::ComputeTargetInfo(const AActor* OwnerActor, const AActor* Target, FVector& OutDir, float& OutAngleDeg, ETargetSide& OutSide) const
{
    if (!OwnerActor || !Target)
    {
        OutDir = FVector::ZeroVector;
        OutAngleDeg = 0.f;
        OutSide = ETargetSide::Center;
        return false;
    }

    FVector OwnerPos = OwnerActor->GetActorLocation();
    FVector TargetPos = Target->GetActorLocation();

    FVector ToTarget = TargetPos - OwnerPos;

    if (bFlattenZ)
    {
        ToTarget.Z = 0.f;
    }

    if (ToTarget.IsNearlyZero())
    {
        OutDir = FVector::ZeroVector;
        OutAngleDeg = 0.f;
        OutSide = ETargetSide::Center;
        return true;
    }

    OutDir = ToTarget.GetSafeNormal();

    FVector Forward = OwnerActor->GetActorForwardVector().GetSafeNormal();
    if (bFlattenZ)
    {
        Forward.Z = 0.f;
        Forward = Forward.GetSafeNormal();
    }

    const float Dot = FMath::Clamp(FVector::DotProduct(Forward, OutDir), -1.f, 1.f);
    OutAngleDeg = FMath::RadiansToDegrees(FMath::Acos(Dot));

    const FVector Up = FVector::UpVector;
    const FVector Cross = FVector::CrossProduct(Forward, OutDir);
    const float Signed = FVector::DotProduct(Cross, Up);

    if (Dot < BehindDotThreshold)
    {
        OutSide = ETargetSide::Behind;
        return true;
    }

    if (Signed > SideEpsilon)
        OutSide = ETargetSide::Right;
    else if (Signed < -SideEpsilon)
        OutSide = ETargetSide::Left;
    else
        OutSide = ETargetSide::Center;

    return true;
}

