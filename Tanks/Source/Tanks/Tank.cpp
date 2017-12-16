// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "Projectile.h"

// Sets default values
ATank::ATank() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay() {
	Super::BeginPlay();
	
}

void ATank::AimAt(FVector HitLocation) {
	if (!ensure(TankAimingComponent)) {
		return;
	}

	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::Fire() {
	if (!ensure(Barrel)) {
		return;
	}

	bool IsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTime_S;

	if (IsReloaded) {
		// spawn projectile at barrel location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}
