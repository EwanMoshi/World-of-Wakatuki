// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class TANKS_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// -1 is max down speed, +1 is max up speed
	void Elevate(float RelativeSpeed);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxDegreesPerSecond = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxElevationDegrees = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MinElevationDegrees = 0.0f;
};
