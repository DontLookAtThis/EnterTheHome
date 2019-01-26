// Fill out your copyright notice in the Description page of Project Settings.

#include "EnterTheHomeGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

#include "Enemy.h"

int AEnterTheHomeGameModeBase::GetAliveEnemies()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);

	int NumberOfAlive = 0;
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		if (Cast<AEnemy>(FoundActors[i])->IsAlive)
		{
			NumberOfAlive++;
		}
	}

	return NumberOfAlive;
}
