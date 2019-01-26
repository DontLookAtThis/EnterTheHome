// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "EnterTheHomeGameModeBase.h"
#include "Enemy.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HoldLocation = CreateDefaultSubobject<USceneComponent>(TEXT("HoldLocation"));
	HoldLocation->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Cast<AEnterTheHomeGameModeBase>(GetWorld()->GetAuthGameMode())->PlayerRef = this;
}

void APlayerCharacter::HoldFurniture()
{
	if (HoldingFurniture)
	{
		HeldFurniture->SetActorLocation(HoldLocation->GetComponentLocation());
	}
}

void APlayerCharacter::XAxisMove(float AxisValue)
{
	FVector Direction = FVector(1.0f, 0.0f, 0.0f) * AxisValue;
	AddMovementInput(Direction);
}

void APlayerCharacter::YAxisMove(float AxisValue)
{
	FVector Direction = FVector(0.0f, 1.0f, 0.0f) * AxisValue;
	AddMovementInput(Direction);
}

void APlayerCharacter::XAxisFacing(float AxisValue)
{
	if (abs(AxisValue) > DirectionDeadZone)
	{
		CurrentFacingDirection.X = AxisValue;
		FVector Right = CurrentFacingDirection.RotateAngleAxis(90, FVector(0.0f, 0.0f, 90.0f));
		FRotator NewRotation = UKismetMathLibrary::MakeRotationFromAxes(CurrentFacingDirection, Right, FVector(0.0f, 0.0f, 1.0f));
		
		SetActorRotation(NewRotation);
	}
}

void APlayerCharacter::YAxisFacing(float AxisValue)
{
	if (abs(AxisValue) > DirectionDeadZone)
	{
		CurrentFacingDirection.Y = AxisValue;
		FVector Right = CurrentFacingDirection.RotateAngleAxis(90, FVector(0.0f, 0.0f, 90.0f));
		FRotator NewRotation = UKismetMathLibrary::MakeRotationFromAxes(CurrentFacingDirection, Right, FVector(0.0f, 0.0f, 1.0f));

		SetActorRotation(NewRotation);
	}
}

void APlayerCharacter::Attack()
{
	if (CanAttack)
	{
		GetWorldTimerManager().SetTimer(AttackCooldownhandle, this, &APlayerCharacter::ResetAttack, AttackCooldown);
		CanAttack = false;
		
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + GetActorForwardVector() * AttackRange;
		FHitResult AttackHit;
		if (GetWorld()->SweepSingleByChannel(AttackHit, StartLocation, EndLocation, GetActorForwardVector().Rotation().Quaternion(), ECollisionChannel::ECC_EngineTraceChannel1, FCollisionShape::MakeBox(FVector(50.0f, 50.0f, 0.0f))))
		{
			Cast<AEnemy>(AttackHit.Actor)->Attacked();
		}
	}
}

void APlayerCharacter::Pickup()
{
	if (HoldingFurniture)
	{
		HoldingFurniture = false;
		HeldFurniture->Held = false;
		HeldFurniture->StartIdleCooldown();
	}
	else
	{
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + GetActorForwardVector() * AttackRange;
		FHitResult AttackHit;
		if (GetWorld()->SweepSingleByChannel(AttackHit, StartLocation, EndLocation, GetActorForwardVector().Rotation().Quaternion(), ECollisionChannel::ECC_EngineTraceChannel1, FCollisionShape::MakeBox(FVector(50.0f, 50.0f, 0.0f))))
		{
			AEnemy* Enemy = Cast<AEnemy>(AttackHit.Actor);

			if (!Enemy->IsAlive)
			{
				HeldFurniture = Enemy;
				HoldingFurniture = true;
				HeldFurniture->Held = false;
			}
		}
	}
}

void APlayerCharacter::ResetAttack()
{
	CanAttack = true;
	GetWorldTimerManager().ClearTimer(AttackCooldownhandle);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HoldFurniture();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("XAxisMove", this, &APlayerCharacter::XAxisMove);
	PlayerInputComponent->BindAxis("YAxisMove", this, &APlayerCharacter::YAxisMove);
	PlayerInputComponent->BindAxis("XAxisFacing", this, &APlayerCharacter::XAxisFacing);
	PlayerInputComponent->BindAxis("YAxisFacing", this, &APlayerCharacter::YAxisFacing);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &APlayerCharacter::Pickup);
}

