// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay() {
	LastFireTime = FPlatformTime::Seconds();

}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	
	if (RoundsLeft <= 0) {
		FiringState = EFiringState::OUT_OF_AMMO;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTime_S) {
		FiringState = EFiringState::RELOADING;
	}
	else if (IsBarrelMoving()) {
		FiringState = EFiringState::AIMING;
	}
	else {
		FiringState = EFiringState::LOCKED;
	}
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}


void UTankAimingComponent::AimAt(FVector HitLocation) {
	if (!ensure(Barrel)) {
		return;
	}

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// calculate OutLaunchVelocity
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0.0f, 0.0f, ESuggestProjVelocityTraceOption::DoNotTrace)) {
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	if (!ensure(Barrel) || !ensure(Turret)) {
		return;
	}
	// calculate difference between barrel rotation and aim direction
	// move barrel the right amount this frame
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);

	// rotate shortest path
	if (FMath::Abs(DeltaRotator.Yaw) < 180) {
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else {
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

bool UTankAimingComponent::IsBarrelMoving() {
	if (!ensure(Barrel)) return false;

	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.1f);
}

void UTankAimingComponent::Fire() {

	if (FiringState == EFiringState::LOCKED || FiringState == EFiringState::AIMING) {
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) {	return; }

		// spawn projectile at barrel location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const {
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const {
	return RoundsLeft;
}
