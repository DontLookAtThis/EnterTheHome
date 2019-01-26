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

	int GetAliveEnemies();
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int MaxEnemyCount = 4.0f;
};
