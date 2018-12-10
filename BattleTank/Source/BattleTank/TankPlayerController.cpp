// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

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

ATank* ATankPlayerController::GetControlledTank() const {
		
	return Cast<ATank>(GetPawn());
}