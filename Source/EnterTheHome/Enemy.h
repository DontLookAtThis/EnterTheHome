// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class ENTERTHEHOME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		class UParticleSystemComponent* StunnedPS;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		class UParticleSystemComponent* PossessedPS;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		class USkeletalMeshComponent* OutsidePossessedMesh;

	void Attacked();
	UFUNCTION(BlueprintCallable)
	void StartIdleCooldown();
	void UnStun();
	void DisableEnemy();
	void DisableAfterStunSpeed();
	void EnableEnemyAlive();
	void CheckReturnPositionSnap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool inPosition;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UStaticMeshComponent* OGLocationOutline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsAlive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool Stunned = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FFloatRange StunTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FFloatRange CooldownTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FTimerHandle IdleCooldownHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FTimerHandle StunCooldownHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FTimerHandle AfterStunSpeedHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool Held = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Health = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float HitDamage = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SprintSpeed = 1300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float OriginalSpeed = 600.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FastEscapeTime = 1.5f;

private:
	float CurrentHealth;
};
