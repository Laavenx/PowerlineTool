// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerlineManager.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class APowerlineActor;
class UPowerlineConnector;

UCLASS(CollapseCategories, HideCategories = (Input, Physics, Collision))
class POWERLINES_API APowerlineManager : public AActor
{
	GENERATED_BODY()

public:
	APowerlineManager();
	
	void AddInstances(APowerlineActor* PowerlineActor);
	void HideInstances(const TArray<int32>& InstancesToChange) const;
	void RemoveInstances(const TArray<int32>& InstancesToRemove);
	void UpdateInstances(APowerlineActor* PowerlineActor) const;

	UFUNCTION(CallInEditor, Category="Config")
	void ClearAllInstances() const;

	// Use to adjust rotation
	UPROPERTY(EditAnywhere, Category="Config")
	FVector UPVectorOfMesh = FVector(0, 0, 1);

	// Length of the cable in CM ( Required to calculate stretching from point A to B )
	UPROPERTY(EditAnywhere, Category="Config")
	float LengthOfCable = 500;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> CableHism;

	UPROPERTY(VisibleAnywhere)
	TArray<int32> FreedInstances;
	
	virtual void BeginPlay() override;
};
