// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

#include "Enemy.h"

void AEnemyAIController::SetNewRandomLocation()
{
	float RandomX = FMath::FRandRange(-800, 1200);
	float RandomY = FMath::FRandRange(-1300, 1300);

	CurrentMoveToLocation = FVector(RandomX, RandomY, 0);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEnemy* ControlledEnemy = Cast<AEnemy>(GetPawn());
	if (ControlledEnemy->IsAlive && !ControlledEnemy->Stunned)
	{
		 EPathFollowingRequestResult::Type Result = MoveToLocation(CurrentMoveToLocation);
		 

		 if (Result == EPathFollowingRequestResult::AlreadyAtGoal || Result == EPathFollowingRequestResult::Failed)
		 {
			 SetNewRandomLocation();
		 }
		 else if (ControlledEnemy->GetVelocity().Size() < 10.0f)
		 {
			 SetNewRandomLocation();
		 }
		 else if (false) // IF close to player, RUUUUN
		 {

		 }

	}
	else
	{
		StopMovement();
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	SetNewRandomLocation();
}
