// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerlineActor.generated.h"

class UPowerlineConnector;
class UHierarchicalInstancedStaticMeshComponent;

UCLASS(CollapseCategories, HideCategories = (Input))
class POWERLINES_API APowerlineActor : public AActor
{
	GENERATED_BODY()

public:
	APowerlineActor();
	
	// Hism cable instances owned by this actor
	UPROPERTY(VisibleAnywhere, Category="Config")
	TArray<int32> HISMInstances;

	FORCEINLINE TArray<int32>& GetHISMInstances();
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<APowerlineActor> TargetMesh;

	// Powerline actor that is connected to this actor
	UPROPERTY(VisibleAnywhere)
	TSoftObjectPtr<APowerlineActor> ConnectedMesh;
	
	UPROPERTY(VisibleAnywhere, Category="Config")
	TArray<UPowerlineConnector*> Connectors;

	void SpawnOrUpdateCableInstancesIfValid();
	void ClearCableInstances();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
};
