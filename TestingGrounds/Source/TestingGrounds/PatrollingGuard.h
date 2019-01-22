// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrollingGuard.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API APatrollingGuard : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
	TArray<AActor*> PatrolPointsCPP;
	
};
