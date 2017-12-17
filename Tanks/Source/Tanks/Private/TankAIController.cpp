// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = (GetPawn());

	if (!ensure(PlayerTank && ControlledTank)) {
		return;
	}

	// move towards player (acceptance radius is in cm)
	MoveToActor(PlayerTank, AcceptanceRadius);

	// aim at the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// fire
	if (AimingComponent->GetFiringState() == EFiringState::LOCKED) {
		AimingComponent->Fire();
	}
	
}


