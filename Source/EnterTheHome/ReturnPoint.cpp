// Fill out your copyright notice in the Description page of Project Settings.

#include "ReturnPoint.h"

void UReturnPoint::ShowPoition()
{
	SetVisibility(true, false);
}

void UReturnPoint::HidePoition()
{
	SetVisibility(false, false);
}

void UReturnPoint::BeginPlay()
{
	Super::BeginPlay();

	SetVisibility(false, false);
}
