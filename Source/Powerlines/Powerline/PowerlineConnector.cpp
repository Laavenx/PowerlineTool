// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerlineConnector.h"

UPowerlineConnector::UPowerlineConnector()
{
	// Doesn't need to tick
	PrimaryComponentTick.bCanEverTick = false;
}

void UPowerlineConnector::BeginPlay()
{
	Super::BeginPlay();
}
