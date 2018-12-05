// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();	
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate!"), *GetOwner()->GetName());
		return;
	}
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, CloseAngle, 0.0f));
}
   

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		
	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > 100.f) {		// TODO make a parameter
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}	

	// Check if it's time to close the door
	if ((LastDoorOpenTime + DoorCloseDelay) < GetWorld()->GetTimeSeconds()) {
		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	
	float TotalMass = 0.f;

	/// find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	/// iterator over adding their masses
	for (auto& OverlappingActor : OverlappingActors) {

		float OverlappingActorMass = OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("object on pressure plate is %s, mass = %f"), *(OverlappingActor->GetName()), OverlappingActorMass);

		TotalMass += OverlappingActorMass;
	}

	UE_LOG(LogTemp, Warning, TEXT("total mass = %f"), TotalMass);

	return TotalMass;
}

