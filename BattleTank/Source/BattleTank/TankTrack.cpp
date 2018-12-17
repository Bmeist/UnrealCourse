// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// calculate the slippage speed
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// work out the required acceleration this frame
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// calculate and apply sideways force (F = ma)
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = TankRoot->GetMass() * CorrectionAcceleration / 2;  //Two tracks

	//TankRoot->AddForceAtLocation(CorrectionForce, GetComponentLocation());
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
};