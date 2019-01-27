// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "EnterTheHomeGameModeBase.h"
#include "Enemy.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GameFramework/CharacterMovementComponent.h"




// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HoldPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPosition"));
	HoldPosition->SetupAttachment(RootComponent);

	BroomstickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Broomstick Mesh"));
	BroomstickMesh->SetupAttachment(GetMesh(), "ArmR");

	BroomstickFlyingPS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BroomstickFlyingPS"));
	BroomstickFlyingPS->SetupAttachment(BroomstickMesh);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Cast<AEnterTheHomeGameModeBase>(GetWorld()->GetAuthGameMode())->PlayerRef = this;

	NormalSpeed = GetCharacterMovement()->MaxWalkSpeed;

	BroomstickMesh->SetVisibility(false);
	NormalBroomPos = BroomstickMesh->RelativeLocation;
	NormalBroomRot = BroomstickMesh->RelativeRotation;
}

void APlayerCharacter::HoldFurniture()
{
	if (HoldingFurniture)
	{
		if (HeldFurniture && HoldPosition)
		{
			HeldFurniture->SetActorLocation(HoldPosition->GetComponentLocation());
			HeldFurniture->SetActorRotation(GetActorRotation());
		}
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
	CurrentInput.X = AxisValue;
	
}

void APlayerCharacter::YAxisFacing(float AxisValue)
{
	CurrentInput.Y = AxisValue;
	/*if (abs(AxisValue) > DirectionDeadZone)
	{
		CurrentFacingDirection.Y = AxisValue;
		FVector Right = CurrentFacingDirection.RotateAngleAxis(90, FVector(0.0f, 0.0f, 90.0f));
		FRotator NewRotation = UKismetMathLibrary::MakeRotationFromAxes(CurrentFacingDirection, Right, FVector(0.0f, 0.0f, 1.0f));

		SetActorRotation(NewRotation);
	}*/
}

void APlayerCharacter::Attack()
{
	if (CanAttack && !IsFlying)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), AttackSound, 0.5f);
		IsAttacking = true;
		GetWorldTimerManager().SetTimer(AttackAnimationHandle, this, &APlayerCharacter::StopAttackAnim, AttackAnimationTime);
		BroomstickMesh->SetVisibility(true);

		CanAttack = false;
		GetWorldTimerManager().SetTimer(AttackCooldownhandle, this, &APlayerCharacter::ResetAttack, AttackCooldown);
		
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = StartLocation + GetActorForwardVector() * AttackRange;
		FHitResult AttackHit;
		FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
		const FName TraceTag("VisibleTrace");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionParams.TraceTag = TraceTag;
		if (GetWorld()->SweepSingleByChannel(AttackHit, StartLocation, EndLocation, GetActorForwardVector().ToOrientationQuat(), ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeBox(FVector(0.0f, 50.0f, 100.0f)), CollisionParams))
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
		if (GetWorld()->SweepSingleByChannel(AttackHit, StartLocation, EndLocation, GetActorForwardVector().Rotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeBox(FVector(50.0f, 100.0f, 0.0f))))
		{
			AEnemy* Enemy = Cast<AEnemy>(AttackHit.Actor);

			if (!Enemy->IsAlive && !Enemy->inPosition)
			{
				HeldFurniture = Enemy;
				HoldingFurniture = true;
				HeldFurniture->Held = true;
				BroomstickMesh->SetVisibility(false);
			}
		}
	}
}

void APlayerCharacter::StartFlying()
{
	if (HoldingFurniture || IsAttacking) return;
	UGameplayStatics::PlaySound2D(GetWorld(), BroomZoomSound, 0.5f);
	IsFlying = true;
	GetCharacterMovement()->MaxWalkSpeed = FlyingSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	BroomstickMesh->RelativeLocation = FlyingBroomPos;
	BroomstickMesh->RelativeRotation = FlyingBroomRot;
	BroomstickMesh->SetVisibility(true);
	BroomstickFlyingPS->Activate();
	BroomstickFlyingPS->SetVisibility(true);
}

void APlayerCharacter::StopFlying()
{
	IsFlying = false;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	BroomstickMesh->SetVisibility(false);

	BroomstickMesh->RelativeLocation = NormalBroomPos;
	BroomstickMesh->RelativeRotation = NormalBroomRot;
	BroomstickFlyingPS->Deactivate();
	BroomstickFlyingPS->SetVisibility(false);
}

void APlayerCharacter::ResetAttack()
{
	CanAttack = true;
	GetWorldTimerManager().ClearTimer(AttackCooldownhandle);
}

void APlayerCharacter::StopAttackAnim()
{
	IsAttacking = false;
	BroomstickMesh->SetVisibility(false);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HoldFurniture();

	if (CurrentInput.Size() > DirectionDeadZone && !IsFlying)
	{
		FVector Right = CurrentInput.RotateAngleAxis(90, FVector(0.0f, 0.0f, 90.0f));
		FRotator NewRotation = UKismetMathLibrary::MakeRotationFromAxes(CurrentInput, Right, FVector(0.0f, 0.0f, 1.0f));

		SetActorRotation(NewRotation);
	}
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
	PlayerInputComponent->BindAction("Flying", IE_Pressed, this, &APlayerCharacter::StartFlying);
	PlayerInputComponent->BindAction("Flying", IE_Released, this, &APlayerCharacter::StopFlying);
}

