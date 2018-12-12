// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	ATank* PlayerTank = GetPlayerTank();
	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("AI controller - No player tank found"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI controller - Player Tank is %s"), *PlayerTank->GetName());
	}

}

void ATankAIController::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if (GetPlayerTank())
	{
		// TODO move towards player

		// Aim towards player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

		// Fire if ready
	}
}

ATank* ATankAIController::GetControlledTank() const {

	auto Pawn = GetPawn();
	if (!Pawn) { return nullptr; }

	return Cast<ATank>(Pawn);
}

ATank* ATankAIController::GetPlayerTank() const {
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	
	return Cast<ATank>(PlayerPawn);
}
