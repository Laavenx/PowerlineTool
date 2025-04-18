// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerlineActor.h"

#include "PowerlineConnector.h"
#include "PowerlineManager.h"
#include "Kismet/GameplayStatics.h"

APowerlineActor::APowerlineActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

TArray<int32>& APowerlineActor::GetHISMInstances()
{
	return HISMInstances;
}

void APowerlineActor::SpawnOrUpdateCableInstancesIfValid()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APowerlineManager::StaticClass());
	if (auto PowerlineManager = Cast<APowerlineManager>(FoundActor))
	{
		if (GetHISMInstances().Num() == Connectors.Num())
		{
			PowerlineManager->UpdateInstances(this);
		}
		else
		{
			PowerlineManager->AddInstances(this);
		}
	}
}

void APowerlineActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (ConnectedMesh.IsValid())
	{
		ConnectedMesh->SpawnOrUpdateCableInstancesIfValid();
	}
	UE_LOG(LogTemp, Display, TEXT("Streamed In"));
}

void APowerlineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Connectors.Empty();
	TArray<USceneComponent*> ChildrenComponents;
	GetRootComponent()->GetChildrenComponents(false, ChildrenComponents);
	for (UActorComponent* Component : ChildrenComponents)
	{
		if (auto Connector = Cast<UPowerlineConnector>(Component))
		{
			Connectors.Add(Connector);
		}
	}

	if (ConnectedMesh.IsValid())
	{
		ConnectedMesh->SpawnOrUpdateCableInstancesIfValid();
	}

	if (TargetMesh.IsValid())
	{
		TargetMesh->ConnectedMesh = this;
		AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APowerlineManager::StaticClass());
		if (auto PowerlineManager = Cast<APowerlineManager>(FoundActor))
		{
			if (GetHISMInstances().Num() == Connectors.Num())
			{
				PowerlineManager->UpdateInstances(this);
			}
			else
			{
				PowerlineManager->AddInstances(this);
			}
		}
	}
}

void APowerlineActor::ClearCableInstances()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APowerlineManager::StaticClass());
	if (auto PowerlineManager = Cast<APowerlineManager>(FoundActor))
	{
		PowerlineManager->HideInstances(GetHISMInstances());
	}
}

void APowerlineActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Display, TEXT("Streamed out"));
	if (ConnectedMesh.IsValid())
	{
		ConnectedMesh->ClearCableInstances();
	}
		
	if (GetHISMInstances().Num() == 0) { return; }
	ClearCableInstances();
}

void APowerlineActor::Destroyed()
{
	Super::Destroyed();

	if (GetWorld()->WorldType == EWorldType::Editor && !HasAnyFlags(RF_Transient))
	{
		AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APowerlineManager::StaticClass());
		auto PowerlineManager = Cast<APowerlineManager>(FoundActor);
		if (ConnectedMesh.IsValid() && PowerlineManager)
		{
			PowerlineManager->RemoveInstances(ConnectedMesh->GetHISMInstances());
			ConnectedMesh->GetHISMInstances().Empty();
			ConnectedMesh->TargetMesh.Reset();
		}

		if (TargetMesh.IsValid() && PowerlineManager)
		{
			PowerlineManager->RemoveInstances(GetHISMInstances());
			TargetMesh->ConnectedMesh.Reset();
		}
	}
}
