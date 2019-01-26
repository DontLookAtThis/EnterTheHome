// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnterTheHomeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEHOME_API AEnterTheHomeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	class APlayerCharacter* PlayerRef;
	TArray<AActor*> FoundActors;

	UFUNCTION(BlueprintCallable)
		void GetAllEnemies();
	UFUNCTION(BlueprintCallable)
		int GetAliveEnemies();
	UFUNCTION(BlueprintCallable)
		int CalculateScore();
	UFUNCTION(BlueprintCallable)
		void RoundEnd();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int MaxEnemyCount = 4.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FTimerHandle RoundEndTimer;

	int RoundTime;
	int FinalScore;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
};
