// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ReturnPoint.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEHOME_API UReturnPoint : public UStaticMeshComponent
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		void ShowPoition();


	UFUNCTION(BlueprintCallable)
		void HidePoition();

	virtual void BeginPlay() override;
};
