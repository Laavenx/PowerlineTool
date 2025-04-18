// Copyright Epic Games, Inc. All Rights Reserved.

#include "PowerlinesGameMode.h"
#include "PowerlinesCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Powerline/PowerlineManager.h"
#include "UObject/ConstructorHelpers.h"

APowerlinesGameMode::APowerlinesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void APowerlinesGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Moves all instances out of sight on game start
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APowerlineManager::StaticClass());
	if (auto PowerlineManager = Cast<APowerlineManager>(FoundActor))
	{
		PowerlineManager->ClearAllInstances();
	}
}
