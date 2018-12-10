// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorEvent.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UDoorEvent::UDoorEvent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorEvent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();	
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate!"), *GetOwner()->GetName());
		return;
	}
}

// Called every frame
void UDoorEvent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		
	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {		// TODO make a parameter
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
	}	
}

float UDoorEvent::GetTotalMassOfActorsOnPlate() {
	
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

