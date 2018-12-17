// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.h"
#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
}

void ATank::Fire() 
{
	if (!ensure(Barrel)) { return; }
	//FPlatformTime::Seconds
	bool isReloaded = (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - LastFireTime) > ReloadTimeInSeconds;

	if (isReloaded)
	{
		/// Spawn projectile at socket location on the barrel

		const FVector& Location = Barrel->GetSocketLocation(FName("Projectile"));
		const FRotator& Rotation = Barrel->GetSocketRotation(FName("Projectile"));
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Location, Rotation, FActorSpawnParameters());

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	}
}

void ATank::IntendMoveForward(float Throw)
{
	//TankMovementComponent->IntendMoveForward(Throw);
}

void ATank::AimAt(FVector HitLocation) 
{
	if (!ensure(TankAimingComponent)) { return; }
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	//TankAimingComponent->SetBarrelReference(BarrelToSet);
	Barrel = BarrelToSet;
}

// Called to bind functionality to input
/*
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/

