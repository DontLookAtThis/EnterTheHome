// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnterTheHomeGameModeBase.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StunTime.SetLowerBound(1.0f);
	StunTime.SetUpperBound(2.0f);
	CooldownTime.SetLowerBound(1.0f);
	CooldownTime.SetUpperBound(60.0f);
}

void AEnemy::Attacked()
{
	if (Health - HitDamage > 0)
	{
		Health = Health - HitDamage;
		if (!Stunned)
		{
			float RandomStunTime = FMath::FRandRange(StunTime.GetLowerBound().GetValue(), StunTime.GetUpperBound().GetValue());
			GetWorldTimerManager().SetTimer(StunCooldownHandle, this, &AEnemy::UnStun, RandomStunTime);

		}
	}
	else
	{
		Health = 0.0f;
		DisableEnemy();
		StartIdleCooldown();
	}
}

void AEnemy::StartIdleCooldown()
{
	GetWorldTimerManager().ClearTimer(IdleCooldownHandle);

	float RandomCooldownTime = FMath::FRandRange(CooldownTime.GetLowerBound().GetValue(), CooldownTime.GetUpperBound().GetValue());
	GetWorldTimerManager().SetTimer(IdleCooldownHandle, this, &AEnemy::EnableEnemyAlive, RandomCooldownTime);
}

void AEnemy::UnStun()
{
	Stunned = false;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	GetWorldTimerManager().SetTimer(AfterStunSpeedHandle, this, &AEnemy::DisableAfterStunSpeed, FastEscapeTime);
	
}

void AEnemy::DisableEnemy()
{
	IsAlive = false;
}

void AEnemy::DisableAfterStunSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
}

void AEnemy::EnableEnemyAlive()
{
	AEnterTheHomeGameModeBase* GM = Cast<AEnterTheHomeGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM->GetAliveEnemies() < GM->MaxEnemyCount && !Held)
	{
		IsAlive = true;
	}
	else
	{
		StartIdleCooldown();
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	StartIdleCooldown();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

