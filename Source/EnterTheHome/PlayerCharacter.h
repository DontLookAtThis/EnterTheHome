// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ENTERTHEHOME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		USceneComponent* HoldPosition;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		UStaticMeshComponent* BroomstickMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		UParticleSystemComponent* BroomstickFlyingPS;


	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		USoundBase* AttackSound;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		USoundBase* BroomZoomSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HoldFurniture();

	void XAxisMove(float AxisValue);
	void YAxisMove(float AxisValue);
	void XAxisFacing(float AxisValue);
	void YAxisFacing(float AxisValue);

	void Attack();
	void Pickup();
	void StartFlying();
	void StopFlying();

	void ResetAttack();

	void StopAttackAnim();

	FVector CurrentInput = FVector();

	FTimerHandle AttackCooldownhandle;

	FTimerHandle AttackAnimationHandle;

	float NormalSpeed;

	FVector NormalBroomPos;
	FRotator NormalBroomRot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackCooldown = 0.7f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanAttack = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackRange = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AEnemy* HeldFurniture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool HoldingFurniture = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DirectionDeadZone = 0.3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FlyingSpeed = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector FlyingBroomPos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FRotator FlyingBroomRot;


	// Animation
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackAnimationTime = 0.8f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsFlying = false;

};
