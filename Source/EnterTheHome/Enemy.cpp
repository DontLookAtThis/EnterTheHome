// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnterTheHomeGameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

void AEnemy::CheckReturnPositionSnap()
{
	float distance = 0.0f;
	FVector temp;
	(GetActorLocation() - OGLocationOutline->GetComponentLocation()).ToDirectionAndLength(temp, distance);

	if (distance < 300.0f)
	{
		FHitResult temp;
		SetActorLocation(OGLocationOutline->GetComponentLocation(), false, &temp, ETeleportType::TeleportPhysics);
		SetActorRotation(OGLocationOutline->GetComponentQuat(), ETeleportType::TeleportPhysics);
		GetMesh()->SetSimulatePhysics(false);
		GetCharacterMovement()->SetActive(false);
		GetCharacterMovement()->SetActive(true);
		inPosition = true;
	}
}

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StunTime.SetLowerBound(1.0f);
	StunTime.SetUpperBound(2.0f);
	CooldownTime.SetLowerBound(1.0f);
	CooldownTime.SetUpperBound(60.0f);
	OGLocationOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ReturnOutline"));
	OGLocationOutline->SetVisibility(false, false);
	inPosition = true;

	StunnedPS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StunnedParticleSystem"));
	StunnedPS->SetupAttachment(RootComponent);
};

void AEnemy::Attacked()
{
	if (CurrentHealth - HitDamage > 0)
	{
		CurrentHealth = CurrentHealth - HitDamage;
		if (!Stunned)
		{
			float RandomStunTime = FMath::FRandRange(StunTime.GetLowerBound().GetValue(), StunTime.GetUpperBound().GetValue());
			GetWorldTimerManager().SetTimer(StunCooldownHandle, this, &AEnemy::UnStun, RandomStunTime);
			Stunned = true;
			StunnedPS->SetVisibility(true);
			StunnedPS->Activate(true);
		}
	}
	else
	{
		CurrentHealth = 0.0f;
		DisableEnemy();
		StartIdleCooldown();
	}
}

void AEnemy::StartIdleCooldown()
{
	GetWorldTimerManager().ClearTimer(IdleCooldownHandle);

	float RandomCooldownTime = FMath::FRandRange(CooldownTime.GetLowerBound().GetValue(), CooldownTime.GetUpperBound().GetValue());
	GetWorldTimerManager().SetTimer(IdleCooldownHandle, this, &AEnemy::EnableEnemyAlive, RandomCooldownTime);
	CurrentHealth = Health;
}

void AEnemy::UnStun()
{
	Stunned = false;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	GetWorldTimerManager().SetTimer(AfterStunSpeedHandle, this, &AEnemy::DisableAfterStunSpeed, FastEscapeTime);
	StunnedPS->Deactivate();
	StunnedPS->SetVisibility(false);
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
		inPosition = false;
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
	FHitResult temp;
	if (OGLocationOutline)
	{
		OGLocationOutline->SetActive(true);
		OGLocationOutline->bAbsoluteLocation = true;
		OGLocationOutline->bAbsoluteRotation = true;

		OGLocationOutline->SetWorldLocation(GetActorLocation(), false, &temp, ETeleportType::TeleportPhysics);
		OGLocationOutline->SetWorldRotation(GetActorRotation(), false, &temp, ETeleportType::TeleportPhysics);
	}
	CurrentHealth = Health;

	StartIdleCooldown();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("TICK"));
	if (Held)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HELD"));
		if (OGLocationOutline)
		{
			if (!OGLocationOutline->IsVisible())
			{
				//UE_LOG(LogTemp, Warning, TEXT("VISIBLE"));
				OGLocationOutline->SetVisibility(true, false);
			}
		}
	}
	else
	{
		if (OGLocationOutline)
		{
			if (OGLocationOutline->IsVisible())
			{
				OGLocationOutline->SetVisibility(false, false);
			}
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

