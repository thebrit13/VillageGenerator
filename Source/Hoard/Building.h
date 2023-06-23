// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "Building.generated.h"

USTRUCT()
struct FTileInfo
{
	GENERATED_BODY()
public:
	UStaticMeshComponent* Mesh = nullptr;
	enum TileType TileType;
	int FloorNumber = 0;
};

enum TileType
{
	FLOOR,
	CEILING,
	WALL,
	ROOF,
	NONTILE
};

UCLASS()
class HOARD_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int BuildingX;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int BuildingY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int TileSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int FloorCeilingWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CeilingHeight;

	UPROPERTY()
	TArray<FTileInfo> CreatedTiles;

	UPROPERTY()
	TArray<UStaticMeshComponent*> CreatedInterior;

	UPROPERTY()
	TArray<UBoxComponent*> CreatedBoxColliders;

	UPROPERTY(EditDefaultsOnly, Category="Building Parts")
	UStaticMesh* FloorTile;

	UPROPERTY(EditDefaultsOnly, Category = "Building Parts")
	UStaticMesh* WallTile;

	UPROPERTY(EditDefaultsOnly, Category = "Building Parts")
	UStaticMesh* WallWindowTile;

	UPROPERTY(EditDefaultsOnly, Category = "Building Parts")
	UStaticMesh* CeilingTile;

	UPROPERTY(EditDefaultsOnly, Category = "Building Parts")
	UStaticMesh* RoofTile;

	UPROPERTY(EditDefaultsOnly, Category = "Building Parts")
	UStaticMesh* RoofTileCorner;

	UPROPERTY(EditDefaultsOnly, Category = "Interior Parts")
	UStaticMesh* Stairs;

	//UPROPERTY(VisibleAnywhere)
	//UStaticMeshComponent* JamesTest;



	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Setup(bool created, int bXOver, int bYOver, UMaterialInstance* wallMat, UMaterialInstance* floorMat);

	UPROPERTY(VisibleAnywhere)
	FVector BLExtents;

	UPROPERTY(VisibleAnywhere)
	FVector TRExtents;


private:

	void CreateMeshComponent(FString name,TileType tileType, int floorNumber, UStaticMesh* mesh,FVector location,FRotator rotation,bool isTile = true);

	void AddBoxColliders();

	void AddCollider(FString colliderName, FVector relativeLocation, FRotator relativeRotation,FVector extents);

	const int NO_WINDOW_CHANCE = 30;
	const int STAIR_SIZE_REQUIREMENT = 5;

	UMaterialInstance* WallTileInstance;
	UMaterialInstance* FloorTileInstance;

	UPROPERTY()
	TArray<FVector> BottomLeftExtent;
	UPROPERTY()
	TArray<FVector> BottomRightExtent;
	UPROPERTY()
	TArray<FVector> TopLeftExtent;
	UPROPERTY()
	TArray<FVector> TopRightExtent;

	int BuildingHeight = 1;

};
