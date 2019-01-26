// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEHOME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	void SetNewRandomLocation();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector CurrentMoveToLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
