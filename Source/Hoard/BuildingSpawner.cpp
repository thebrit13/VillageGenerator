// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSpawner.h"

// Sets default values
ABuildingSpawner::ABuildingSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParentRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Parent Root Component"));
	SetRootComponent(ParentRootComponent);

	BottomLeftExtents = CreateDefaultSubobject<USceneComponent>(TEXT("BottomLeft"));
	BottomLeftExtents->SetupAttachment(RootComponent);
	TopRightExtents = CreateDefaultSubobject<USceneComponent>(TEXT("TopRight"));
	TopRightExtents->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABuildingSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	CreateBuildings();
}

// Called every frame
void ABuildingSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingSpawner::CreateBuildings()
{
	int sizeX = FMath::Abs(BottomLeftExtents->GetComponentLocation().X - TopRightExtents->GetComponentLocation().X)/TILE_SIZE;
	int sizeY = FMath::Abs(BottomLeftExtents->GetComponentLocation().Y - TopRightExtents->GetComponentLocation().Y)/TILE_SIZE;

	FVector startingSpawn = BottomLeftExtents->GetComponentLocation();

	UE_LOG(LogTemp, Warning, TEXT("START %s"), *startingSpawn.ToString());

	int SafetyAmount = sizeX * sizeY * 1.5f;
	int SafetyCount = 0;

	bool firstRow = true;

	while(true)
	{
		int buildingSizeX = FMath::RandRange(MIN_BUILDING_SIZE, MAX_BUILDING_SIZE);
		int buildingSizeY = FMath::RandRange(MIN_BUILDING_SIZE, MAX_BUILDING_SIZE);

		FVector spawnPoint = startingSpawn + FVector((buildingSizeX * TILE_SIZE)/2.0f, (-buildingSizeY * TILE_SIZE)/2.0f, 0);

		if (firstRow)
		{
			spawnPoint.Y -= (FMath::RandRange(0, 2) * TILE_SIZE);
		}

		//To avoid a infinite loop...just in case
		SafetyCount++;

		if (spawnPoint.Y < TopRightExtents->GetComponentLocation().Y || SafetyCount > SafetyAmount)
		{
			return;
		}

		if (!CanBuildHere(spawnPoint, buildingSizeX, buildingSizeY, TopRightExtents->GetComponentLocation()))
		{
			if (startingSpawn.X + TILE_SIZE < TopRightExtents->GetComponentLocation().X)
			{
				startingSpawn.X += TILE_SIZE*2;
			}
			else
			{
				startingSpawn.X = BottomLeftExtents->GetComponentLocation().X + (FMath::RandRange(0, 2) * TILE_SIZE);
				startingSpawn.Y -= TILE_SIZE * 2;
				firstRow = false;
			}
			continue;
		}

		//UE_LOG(LogTemp, Warning, TEXT("SPAWN %s"), *spawnPoint.ToString());

		FBuildingStruct buildingStruct;

		ABuilding* buildingRef = nullptr;
		//Occasionally, just block out the space
		if (FMath::RandRange(0, 100) > NoSpawnPercentage)
		{
			buildingRef = GetWorld()->SpawnActor<ABuilding>(Building, spawnPoint, FRotator::ZeroRotator);
			if (buildingRef)
			{
				buildingRef->Setup(true, buildingSizeX, buildingSizeY, 
					WallTileOptions[FMath::RandRange(0, FloorTileOptions.Num() - 1)],
					FloorTileOptions[FMath::RandRange(0,FloorTileOptions.Num()-1)]);
				buildingStruct.BuildingRef = buildingRef;
			}
		}


		buildingStruct.BottomLeftExtent = spawnPoint + FVector((-buildingSizeX / 2.0f)*TILE_SIZE, (buildingSizeY / 2.0f)*TILE_SIZE, 0);
		buildingStruct.TopRightExtent = spawnPoint + FVector((buildingSizeX / 2.0f)*TILE_SIZE, (-buildingSizeY / 2.0f)*TILE_SIZE, 0);
		
		if (buildingRef)
		{
			buildingRef->BLExtents = buildingStruct.BottomLeftExtent;
			buildingRef->TRExtents = buildingStruct.TopRightExtent;
		}
		BuildingInfo.Add(buildingStruct);

		if (startingSpawn.X + TILE_SIZE < TopRightExtents->GetComponentLocation().X)
		{
			startingSpawn.X = buildingStruct.TopRightExtent.X + (TILE_SIZE*2);
		}
		else
		{
			startingSpawn.X = BottomLeftExtents->GetComponentLocation().X + (FMath::RandRange(0,2) * TILE_SIZE);
			startingSpawn.Y -= TILE_SIZE * 2;
			firstRow = false;
		}
	}
}

bool ABuildingSpawner::CanBuildHere(FVector proposedLocation, int proposedSizeX, int proposedSizeY, FVector topRightMax)
{
	FVector proposedBL = proposedLocation + FVector((-proposedSizeX / 2.0f) * TILE_SIZE, (proposedSizeY / 2.0f) * TILE_SIZE, 0);
	FVector proposedTR = proposedLocation + FVector((proposedSizeX / 2.0f) * TILE_SIZE, (-proposedSizeY / 2.0f) * TILE_SIZE, 0);

	//Big bounds check
	if (proposedTR.X >= topRightMax.X || proposedTR.Y <= topRightMax.Y)
	{
		return false;
	}

	TArray<FVector> PointsToCheck;
	PointsToCheck.Add(proposedLocation);
	PointsToCheck.Add(proposedBL);
	PointsToCheck.Add(proposedTR);
	PointsToCheck.Add(FVector(proposedBL.X, proposedTR.Y, 0));
	PointsToCheck.Add(FVector(proposedTR.X, proposedBL.Y, 0));

	for (int i = 0; i < BuildingInfo.Num(); i++)
	{
		for (int j = 0; j < PointsToCheck.Num(); j++)
		{
			if (IsPointInside(PointsToCheck[j], BuildingInfo[i].BottomLeftExtent, BuildingInfo[i].TopRightExtent))
			{
				return false;
			}
		}
	}

	for (int i = 0; i < BuildingInfo.Num(); i++)
	{
		if (IsPointInside(BuildingInfo[i].BottomLeftExtent, proposedBL, proposedTR))
		{
			return false;
		}
		if (IsPointInside(BuildingInfo[i].TopRightExtent, proposedBL, proposedTR))
		{
			return false;
		}
		
		FVector tempPoint1 = BuildingInfo[i].BottomLeftExtent;
		tempPoint1.Y = BuildingInfo[i].TopRightExtent.Y;
		FVector tempPoint2 = BuildingInfo[i].TopRightExtent;
		tempPoint2.Y = BuildingInfo[i].BottomLeftExtent.Y;
		if (IsPointInside(tempPoint1, proposedBL, proposedTR))
		{
			return false;
		}
		if (IsPointInside(tempPoint2, proposedBL, proposedTR))
		{
			return false;
		}
	}
	return true;
}

bool ABuildingSpawner::IsPointInside(FVector point, FVector bottomLeft, FVector topRight)
{
	if (bottomLeft.X <= point.X
		&& topRight.X >= point.X
		&& bottomLeft.Y >= point.Y
		&& topRight.Y <= point.Y)
	{
		return true;
	}
	return false;
}

