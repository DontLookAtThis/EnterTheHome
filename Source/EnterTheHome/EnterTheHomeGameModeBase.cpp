// Fill out your copyright notice in the Description page of Project Settings.

#include "EnterTheHomeGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

#include "Enemy.h"
#include "Engine/World.h"
#include "TimerManager.h"

void AEnterTheHomeGameModeBase::GetAllEnemies()
{
	FoundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundActors);
}

int AEnterTheHomeGameModeBase::GetAliveEnemies()
{
	GetAllEnemies();
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

int AEnterTheHomeGameModeBase::CalculateScore()
{
	GetAllEnemies();
	int NumberInPos = 0;
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		if (Cast<AEnemy>(FoundActors[i])->inPosition)
		{
			NumberInPos++;
		}
	}
	return NumberInPos;
}

void AEnterTheHomeGameModeBase::RoundEnd()
{
	FinalScore = CalculateScore();
	UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetFName(), true);
}

void AEnterTheHomeGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnterTheHomeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	RoundTime = 60.0f;
	FinalScore = 0.0f;
	GetWorldTimerManager().SetTimer(RoundEndTimer, this, &AEnterTheHomeGameModeBase::RoundEnd, RoundTime);
	
}
