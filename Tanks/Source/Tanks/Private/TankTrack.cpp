// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay() {
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::ApplySidewaysForce() {
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// calculate requried acceleration this tick to correct (note -SlippageSpeed because we apply in opposite direction)
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorrectionAceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// calculate and apply sideways force
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAceleration) / 2; // divide by 2 because there are 2 tracks

																			  // apply force to the tank
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.0f;
}

void UTankTrack::SetThrottle(float Throttle) {
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1.0f, 1.0f);
}

void UTankTrack::DriveTrack() {
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


