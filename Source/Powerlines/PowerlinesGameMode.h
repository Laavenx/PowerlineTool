// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PowerlinesGameMode.generated.h"

class APowerlineManager;

UCLASS(minimalapi)
class APowerlinesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APowerlinesGameMode();

	virtual void BeginPlay() override;
};



