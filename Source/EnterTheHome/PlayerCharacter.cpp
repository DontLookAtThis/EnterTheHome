// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "EnterTheHomeGameModeBase.h"
#include "Enemy.h"

#include "Kismet/KismetMathLibrary.h"




#include "Engine.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HoldPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPosition"));
	HoldPosition->SetupAttachment(RootComponent);

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
		if (HeldFurniture && HoldPosition) HeldFurniture->SetActorLocation(HoldPosition->GetComponentLocation());
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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Attempting attack!");
		GetWorldTimerManager().SetTimer(AttackCooldownhandle, this, &APlayerCharacter::ResetAttack, AttackCooldown);
		CanAttack = false;
		
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + GetActorForwardVector() * AttackRange;
		FHitResult AttackHit;
		FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
		const FName TraceTag("VisibleTrace");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionParams.TraceTag = TraceTag;
		if (GetWorld()->SweepSingleByChannel(AttackHit, StartLocation, EndLocation, GetActorForwardVector().ToOrientationQuat(), ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeBox(FVector(0.0f, 50.0f, 50.0f)), CollisionParams))
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
		HeldFurniture->CheckReturnPositionSnap();
	}
	else
	{
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + GetActorForwardVector() * AttackRange;
		FHitResult AttackHit;
		if (GetWorld()->SweepSingleByChannel(AttackHit, StartLocation, EndLocation, GetActorForwardVector().Rotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeBox(FVector(50.0f, 50.0f, 0.0f))))
		{
			AEnemy* Enemy = Cast<AEnemy>(AttackHit.Actor);

			if (!Enemy->IsAlive)
			{
				HeldFurniture = Enemy;
				HoldingFurniture = true;
				HeldFurniture->Held = true;
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

