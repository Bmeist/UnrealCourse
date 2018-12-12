// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"

void ATankPlayerController::BeginPlay() 
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Player controller begin play"));

	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("No controlled tank found"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GetPawn returned %s"), *GetPawn()->GetName());
	}
	
}

void ATankPlayerController::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {
		
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair() {
	if (!GetControlledTank()) { return; }

	FVector HitLocation;  // Out parameter
	if (GetSightRayHitLocation(HitLocation)) { // has side-effect, is going to line trace
		//UE_LOG(LogTemp, Warning, TEXT("Hit something at: %s"), *HitLocation.ToString());

		// Tell controlled tank to aim at this point
		GetControlledTank()->AimAt(HitLocation);		
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
		if (GetLookVectorHitLocation(LookDirection, OutHitLocation)) {
			return true;
		}
		else {
			return false;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Hit something at: %s"), *OutHitLocation.ToString());
	}
	else {
		return false;
	}

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
			ECollisionChannel::ECC_Visibility)
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
