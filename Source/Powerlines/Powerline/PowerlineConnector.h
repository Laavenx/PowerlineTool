// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PowerlineConnector.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POWERLINES_API UPowerlineConnector : public USceneComponent
{
	GENERATED_BODY()

public:
	UPowerlineConnector();

protected:
	virtual void BeginPlay() override;
	
};