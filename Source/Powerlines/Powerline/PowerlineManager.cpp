// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerlineManager.h"

#include "PowerlineActor.h"
#include "PowerlineConnector.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

APowerlineManager::APowerlineManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsSpatiallyLoaded = false;

	CableHism = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("CableHism"));
	SetRootComponent(CableHism);
}

void APowerlineManager::AddInstances(APowerlineActor* PowerlineActor)
{
	if (!CableHism->GetStaticMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("No Cable mesh set for CableHism"))
		return;
	}

	APowerlineActor* TargetActor = PowerlineActor->TargetMesh.Get();
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No mesh to connect to set"))
		return;
	}

	if (PowerlineActor->GetHISMInstances().Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instances already spawned"))
		return;
	}

	auto& InConnectors = PowerlineActor->Connectors;
	auto& TargetConnectors = TargetActor->Connectors;
	
	for (int32 Iterator = 0; Iterator < PowerlineActor->Connectors.Num(); Iterator++)
	{
		UPowerlineConnector* NextConnector = TargetConnectors.IsValidIndex(Iterator) ? TargetConnectors[Iterator] : nullptr;
		if (NextConnector)
		{
			const FVector Loc1 = InConnectors[Iterator]->GetComponentLocation();
			const FVector Loc2 = NextConnector->GetComponentLocation();
			const float DistanceBetweenPoints = FVector::Distance(Loc1, Loc2);

			FVector Direction = (Loc2 - Loc1).GetSafeNormal();
			FQuat RotationQuat = FRotationMatrix::MakeFromZX(Direction, UPVectorOfMesh).ToQuat();
			
			FTransform WorldTransform;
			WorldTransform.SetLocation(InConnectors[Iterator]->GetComponentLocation());
			WorldTransform.SetRotation(RotationQuat);
			WorldTransform.SetScale3D(FVector(1.f, 1.f, DistanceBetweenPoints / LengthOfCable));

			if (FreedInstances.Num() > 0)
			{
				int32 IndexToUse = FreedInstances.Pop(EAllowShrinking::No);
				CableHism->UpdateInstanceTransform(IndexToUse, WorldTransform, true, true, true);
				PowerlineActor->GetHISMInstances().Add(IndexToUse);
				continue;
			}
			int32 InstanceID = CableHism->AddInstance(WorldTransform, true);
			PowerlineActor->GetHISMInstances().Add(InstanceID);
		}
	}
}

void APowerlineManager::HideInstances(const TArray<int32>& InstancesToChange) const
{
	// Move instance out of sight instead of deleting it
	FTransform WorldTransform;
	WorldTransform.SetLocation(FVector(0, 0, -1000000));
	WorldTransform.SetRotation(FRotator(0, 0, 0).Quaternion());
	WorldTransform.SetScale3D(FVector(0.f, 0.f, 0.f));
	
	for (const int32 InstanceToChange : InstancesToChange)
	{
		CableHism->UpdateInstanceTransform(InstanceToChange, WorldTransform, true, true, true);
	}
}

void APowerlineManager::RemoveInstances(const TArray<int32>& InstancesToRemove)
{
	// Move instance out of sight instead of deleting it
	FTransform WorldTransform;
	WorldTransform.SetLocation(FVector(0, 0, -1000000));
	WorldTransform.SetRotation(FRotator(0, 0, 0).Quaternion());
	WorldTransform.SetScale3D(FVector(0.f, 0.f, 0.f));
	
	for (const int32 InstanceToChange : InstancesToRemove)
	{
		FreedInstances.Add(InstanceToChange);
		CableHism->UpdateInstanceTransform(InstanceToChange, WorldTransform, true, true, true);
	}
}

void APowerlineManager::UpdateInstances(APowerlineActor* PowerlineActor) const
{
	if (!CableHism->GetStaticMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("No Cable mesh set for CableHism"))
		return;
	}

	APowerlineActor* TargetActor = PowerlineActor->TargetMesh.Get();
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No mesh to connect to set or invalid number of connectors"))
		return;
	}

	auto& InConnectors = PowerlineActor->Connectors;
	auto& NextConnectors = TargetActor->Connectors;

	for (int32 Iterator = 0; Iterator < PowerlineActor->Connectors.Num(); Iterator++)
	{
		UPowerlineConnector* NextConnector = NextConnectors.IsValidIndex(Iterator) ? NextConnectors[Iterator] : nullptr;
		if (NextConnector)
		{
			const FVector Loc1 = InConnectors[Iterator]->GetComponentLocation();
			const FVector Loc2 = NextConnector->GetComponentLocation();
			const float DistanceBetweenPoints = FVector::Distance(Loc1, Loc2);

			FVector Direction = (Loc2 - Loc1).GetSafeNormal();
			FQuat RotationQuat = FRotationMatrix::MakeFromZX(Direction, UPVectorOfMesh).ToQuat();
			
			FTransform WorldTransform;
			WorldTransform.SetLocation(InConnectors[Iterator]->GetComponentLocation());
			WorldTransform.SetRotation(RotationQuat);
			WorldTransform.SetScale3D(FVector(1.f, 1.f, DistanceBetweenPoints / LengthOfCable));
			CableHism->UpdateInstanceTransform(PowerlineActor->GetHISMInstances()[Iterator], WorldTransform, true, true, true);
		}
	}
}

void APowerlineManager::ClearAllInstances() const
{
	// Move instance out of sight instead of deleting it
	FTransform WorldTransform;
	WorldTransform.SetLocation(FVector(0, 0, -1000000));
	WorldTransform.SetRotation(FRotator(0, 0, 0).Quaternion());
	WorldTransform.SetScale3D(FVector(0.f, 0.f, 0.f));

	int32 NumInstances = CableHism->GetInstanceCount();
	CableHism->BatchUpdateInstancesTransform(0, CableHism->GetInstanceCount(), WorldTransform, true, true, true);
}

void APowerlineManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerlineActor::StaticClass(), FoundActors);
	UE_LOG(LogTemp, Warning, TEXT("Found PowerlineActors on BeginPlay: %i"), FoundActors.Num());
}
