// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay() 
{
	Super::BeginPlay();
	if (!GetPawn()) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossesedTank = Cast<ATank>(InPawn);
		if (!ensure(PossesedTank)) { return; }

		// Subscribe our local method to the tank's death event
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnDeath);
	}
}

void ATankPlayerController::OnDeath()
{
	StartSpectatingOnly();
	UE_LOG(LogTemp, Warning, TEXT("Player Controller OnDeath: Tank Died"));
}

void ATankPlayerController::AimTowardsCrosshair() {

	if (!GetPawn()) { return; }  // e.g. if not possesing
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation;  // Out parameter
	if (GetSightRayHitLocation(HitLocation)) { // has side-effect, is going to line trace
		//UE_LOG(LogTemp, Warning, TEXT("Hit something at: %s"), *HitLocation.ToString());

		// Tell controlled tank to aim at this point
		AimingComponent->AimAt(HitLocation);
	}	
}

/// Get world location if linetrace through crosshair, true if it hits the landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {

	// Find crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);   // out params

	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Screen location: %s"), *ScreenLocation.ToString());

	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) 
	{
		// line trace along that look direction and see what we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Hit something at: %s"), *OutHitLocation.ToString());
	}
	
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	// de-project the screen position of the crosshair to a world direction
	FVector CameraWorldLocation; // to discard
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const {

	FCollisionQueryParams Params;
	FCollisionResponseParams ResponseParams;

	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Camera)
		)
	{

		OutHitLocation = HitResult.Location;
		return true;
	}
	else {
		OutHitLocation = FVector(0.0);
		return false;
	}	

}
