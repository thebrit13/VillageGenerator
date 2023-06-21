// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.h"
#include "Components/SceneComponent.h"
#include "Math/IntRect.h"
#include "BuildingSpawner.generated.h"

USTRUCT()
struct FBuildingStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ABuilding* BuildingRef = nullptr;

	FVector BottomLeftExtent = FVector::ZeroVector;
	FVector TopRightExtent = FVector::ZeroVector;
};

UCLASS()
class HOARD_API ABuildingSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuilding> Building;

	UPROPERTY()
	USceneComponent* ParentRootComponent;

	UPROPERTY(EditInstanceOnly)
	USceneComponent* BottomLeftExtents;

	UPROPERTY(EditInstanceOnly)
	USceneComponent* TopRightExtents;

	UPROPERTY(EditDefaultsOnly,Category="Materials Instances")
	TArray<UMaterialInstance*> WallTileOptions;

	UPROPERTY(EditDefaultsOnly, Category = "Materials Instances")
	TArray<UMaterialInstance*> FloorTileOptions;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void CreateBuildings();

	TArray<FBuildingStruct> BuildingInfo;

	const int TILE_SIZE = 100;

	bool CanBuildHere(FVector proposedLocation, int proposedSizeX, int proposedSizeY,FVector topRightMax);

	bool IsPointInside(FVector point, FVector bottomLeft, FVector topRight);

	const int MIN_BUILDING_SIZE = 4;
	const int MAX_BUILDING_SIZE = 8;

	const int NoSpawnPercentage = 10;
};



