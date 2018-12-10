// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AI controller begin play"));

	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("AI controller - No controlled tank found"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI controller - GetPawn returned %s"), *GetPawn()->GetName());
	}

}

ATank* ATankAIController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());
}
