// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	//JamesTest = NewObject<UStaticMeshComponent>(this, TEXT("James Test"));
	////JamesTest = NewObject<UStaticMeshComponent>((UObject*)GetTransientPackage(), UStaticMeshComponent::StaticClass());
	////Owner->AddInstanceComponent(JamesTest);
	//
	//JamesTest->RegisterComponent();
	////Owner->Modify();
	//JamesTest->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	//JamesTest->SetStaticMesh(FloorTile);
}

void ABuilding::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Setup(false,-1,-1,nullptr, nullptr);
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuilding::Setup(bool created,int BuildingXOver,int BuildingYOver,UMaterialInstance* wallMat, UMaterialInstance* floorMat)
{
	if (created)
	{
		BuildingX = BuildingXOver;
		BuildingY = BuildingYOver;
	}

	if (BuildingY >= STAIR_SIZE_REQUIREMENT)// || BuildingY >= STAIR_SIZE_REQUIREMENT)
	{
		BuildingHeight = FMath::RandRange(1, 3);
	}

	WallTileInstance = wallMat;
	FloorTileInstance = floorMat;

	float ActualZPosition = GetActorLocation().Z;

	FVector bottomRightTilePosition;

	//UE_LOG(LogTemp, Warning, TEXT("%d %d"), BuildingX, BuildingY);

	for (int floorNumber = 0; floorNumber < BuildingHeight; floorNumber++)
	{
		FVector nextRelativeSpawn = FVector::Zero();
		nextRelativeSpawn.X = ((-BuildingX / 2.0f) * TileSize) + (TileSize / 2.0f);
		nextRelativeSpawn.Y = ((-BuildingY / 2.0f) * TileSize) + (TileSize / 2.0f);
		nextRelativeSpawn.Z = ActualZPosition + (floorNumber * CeilingHeight);

		//Floor and Ceiling
		for (int i = 0; i < BuildingX; i++)
		{			
			for (int j = 0; j < BuildingY; j++)
			{
				FString tempString1 = FString::Printf(TEXT("TileF_%d_%d_%d"), floorNumber, i, j);
				if (floorNumber == 0)
				{
					CreateMeshComponent(tempString1, TileType::FLOOR, floorNumber, FloorTile, nextRelativeSpawn + FVector(0, 0, FloorCeilingWidth / 2.0f), FRotator::ZeroRotator);
					bottomRightTilePosition = nextRelativeSpawn;
				}

				if (BuildingHeight > 1 && floorNumber < BuildingHeight-1 &&
					i == 0 && j > BuildingY - STAIR_SIZE_REQUIREMENT && j < BuildingY - 1)
				{

				}
				else
				{
					FString tempString2 = FString::Printf(TEXT("TileC_%d_%d_%d"), floorNumber, i, j);
					CreateMeshComponent(tempString2, TileType::CEILING, floorNumber, CeilingTile, nextRelativeSpawn + FVector(0, 0, CeilingHeight - (5 * floorNumber)), FRotator::ZeroRotator);
				}



				float ceilingZ = CeilingHeight + 10 - (floorNumber*5);
				if (floorNumber == BuildingHeight - 1)
				{
					FString tempString3 = FString::Printf(TEXT("TileR_%d_%d_%d"), floorNumber, i, j);
					//TL
					if (i == 0 && j == 0)
					{
						TopLeftExtent.Add(nextRelativeSpawn);
						CreateMeshComponent(tempString3, TileType::ROOF, floorNumber, RoofTileCorner, nextRelativeSpawn + FVector(0, 0, ceilingZ), FRotator(0, 90, 0));
					}
					//BR
					else if (i == BuildingX - 1 && j == BuildingY - 1)
					{
						BottomRightExtent.Add(nextRelativeSpawn);
						CreateMeshComponent(tempString3, TileType::ROOF, floorNumber, RoofTileCorner, nextRelativeSpawn + FVector(0, 0, ceilingZ), FRotator(0, -90, 0));
					}
					//BL
					else if (i == 0 && j == BuildingY - 1)
					{
						BottomLeftExtent.Add(nextRelativeSpawn);
						CreateMeshComponent(tempString3, TileType::ROOF, floorNumber, RoofTileCorner, nextRelativeSpawn + FVector(0, 0, ceilingZ), FRotator::ZeroRotator);
					}
					//TR
					else if (i == BuildingX - 1 && j == 0)
					{
						TopRightExtent.Add(nextRelativeSpawn);
						CreateMeshComponent(tempString3, TileType::ROOF, floorNumber, RoofTileCorner, nextRelativeSpawn + FVector(0, 0, ceilingZ), FRotator(0, -180, 0));
					}
					else
					{
						FRotator roofRot = FRotator::ZeroRotator;
						if (i == 0)
						{
							roofRot = FRotator(0, 90, 0);
						}
						else if (i == BuildingX - 1)
						{
							roofRot = FRotator(0, -90, 0);
						}
						else if (j == 0)
						{
							roofRot = FRotator(0, -180, 0);
						}
						CreateMeshComponent(tempString3, TileType::ROOF, floorNumber, RoofTile, nextRelativeSpawn + FVector(0, 0, ceilingZ), roofRot);
					}
				}

				nextRelativeSpawn.Y += TileSize;
			}
			nextRelativeSpawn.X += TileSize;
			nextRelativeSpawn.Y = ((-BuildingY / 2.0f) * TileSize) + (TileSize / 2.0f);
		}

		//UE_LOG(LogTemp, Warning, TEXT("DONE"));

		//Walls
		FVector topLeft = CreatedTiles[0].Mesh->GetRelativeLocation() + FVector(0, 0, floorNumber * CeilingHeight);
		FVector bottomRight = bottomRightTilePosition;

		topLeft.Z -= 5 * floorNumber;
		bottomRight.Z = topLeft.Z;

		FVector spawnLoc = topLeft;
		FVector spawnLoc2 = bottomRight;
		for (int i = 0; i < BuildingX; i++)
		{
			UStaticMesh* mesh = WallTile;
			if (FMath::RandRange(0, 100) < WINDOW_CHANCE)
			{
				mesh = WallWindowTile;
			}

			CreateMeshComponent(FString::Printf(TEXT("TileW1_%d_%d"), floorNumber, i), TileType::WALL, floorNumber, mesh,
				spawnLoc - FVector(0, .1f, 0), FRotator::ZeroRotator);
			CreateMeshComponent(FString::Printf(TEXT("TileW2_%d_%d"), floorNumber, i), TileType::WALL, floorNumber, mesh,
				spawnLoc2 + FVector(0, .1f, 0), FRotator(0, 180, 0));
			spawnLoc.X += TileSize;
			spawnLoc2.X -= TileSize;
		}

		spawnLoc = topLeft;
		spawnLoc2 = bottomRight;
		for (int i = 0; i < BuildingY; i++)
		{
			UStaticMesh* mesh = WallTile;
			if (FMath::RandRange(0, 100) < WINDOW_CHANCE)
			{
				mesh = WallWindowTile;
			}

			CreateMeshComponent(FString::Printf(TEXT("TileW3_%d_%d"), floorNumber, i), TileType::WALL,floorNumber, mesh,
				spawnLoc - FVector(.1f, 0, 0), FRotator(0, 270, 0));
			CreateMeshComponent(FString::Printf(TEXT("TileW4_%d_%d"), floorNumber, i), TileType::WALL, floorNumber, mesh,
				spawnLoc2 + FVector(.1f, 0, 0), FRotator(0, 90, 0));
			spawnLoc.Y += TileSize;
			spawnLoc2.Y -= TileSize;
		}

		//Create door (for now, will always be top right)
		if (floorNumber == 0)
		{
			CreatedTiles[CreatedTiles.Num() - 1].Mesh->DestroyComponent();
			CreatedTiles.RemoveAt(CreatedTiles.Num() - 1);
		}

		//Create stairs
		if (floorNumber != BuildingHeight - 1)
		{
			FVector spawnPos = topLeft;
			spawnPos.Y = bottomRight.Y - (TileSize / 2.0f);
			CreateMeshComponent(FString::Printf(TEXT("Stair_%d"), floorNumber), TileType::NONTILE, floorNumber, Stairs, spawnPos, FRotator(0,180,0), false);
		}
	}

	AddBoxColliders();
}

void ABuilding::CreateMeshComponent(FString name,TileType tileType, int floorNumber, UStaticMesh* mesh,FVector location,FRotator rotation,bool isTile)
{
	UStaticMeshComponent* tempSM = NewObject<UStaticMeshComponent>(this, FName(*name));
	tempSM->RegisterComponent();
	tempSM->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	tempSM->SetStaticMesh(mesh);
	tempSM->SetRelativeLocation(location);
	tempSM->SetRelativeRotation(rotation);

	switch (tileType)
	{
		case FLOOR:
			tempSM->SetMaterial(0, FloorTileInstance);
			break;
		case CEILING:
			//tempSM->SetMaterial(1, WallTileInstance);
			break;
		case WALL:
			tempSM->SetMaterial(2, WallTileInstance);
			break;
		default:
			break;
	}
	
	if (isTile)
	{
		FTileInfo tileInfo;
		tileInfo.Mesh = tempSM;
		tileInfo.TileType = tileType;
		tileInfo.FloorNumber = floorNumber;
		CreatedTiles.Add(tileInfo);
	}
	else
	{
		CreatedInterior.Add(tempSM);
	}

}

void ABuilding::AddBoxColliders()
{
	float XSize = BuildingX * TileSize;
	float YSize = BuildingY * TileSize;

	//Floor
	AddCollider("Floor_Collider", FVector::ZeroVector, FRotator::ZeroRotator, FVector(XSize / 2.0f, YSize / 2.0f, FloorCeilingWidth / 2.0f));

	for (int i = 1; i <= BuildingHeight; i++)
	{
		//Ceiling
		FVector topLeft = CreatedTiles[0].Mesh->GetRelativeLocation() + FVector(0, 0, i * CeilingHeight);
		topLeft.Z -= 5.0f * i;

		if (i == BuildingHeight)
		{
			AddCollider(FString::Printf(TEXT("CeilingCollider_%d"), i), FVector(0, 0, topLeft.Z-5.0f), FRotator::ZeroRotator, FVector(XSize / 2.0f, YSize / 2.0f, 5));
		}
		else
		{
			AddCollider(FString::Printf(TEXT("CeilingCollider_%d"), i), FVector(TileSize / 2.0f, 0, topLeft.Z-5.0f), FRotator::ZeroRotator, FVector((XSize / 2.0f) - (TileSize / 2.0f), YSize / 2.0f, 5));
		}

		//Extras - to fill in tiles around stairs
		if (i == BuildingHeight-1)
		{
			AddCollider(FString::Printf(TEXT("CeilingColliderExtra_%d"), i), topLeft + FVector(0, TileSize * (BuildingY - 1), -5.0f), FRotator::ZeroRotator, FVector(TileSize / 2.0f, TileSize / 2.0f, 5));
		}
		if (i < BuildingHeight)
		{
			for (int j = 0; j < (BuildingY - STAIR_SIZE_REQUIREMENT); j++)
			{
				AddCollider(FString::Printf(TEXT("CeilingColliderExtra_%d%d"), i, j), topLeft + FVector(0, TileSize * j, -5.0f), FRotator::ZeroRotator, FVector(TileSize / 2.0f, TileSize / 2.0f, 5));
			}
		}
		
		//Walls
		int wallAdjust = i - 1;
		float wallZPos = topLeft.Z - (CeilingHeight / 2.0f);
		AddCollider(FString::Printf(TEXT("Wall_Collider_Bottom_%d"), wallAdjust), FVector(0, (YSize / 2.0f) - 5, wallZPos), FRotator::ZeroRotator, FVector(XSize / 2.0f, 5, CeilingHeight / 2.0f));
		AddCollider(FString::Printf(TEXT("Wall_Collider_Top_%d"), wallAdjust), FVector(0, -(YSize / 2.0f) + 5, wallZPos), FRotator::ZeroRotator, FVector(XSize / 2.0f, 5, CeilingHeight / 2.0f));
		AddCollider(FString::Printf(TEXT("Wall_Collider_Left_%d"), wallAdjust), FVector(-(XSize / 2.0f) + 5, 0, wallZPos), FRotator(0, 90, 0), FVector(YSize / 2.0f, 5, CeilingHeight / 2.0f));
		if (wallAdjust == 0)
		{
			AddCollider(FString::Printf(TEXT("Wall_Collider_Right_%d"), wallAdjust), FVector((XSize / 2.0f) - 5, TileSize / 2.0f, wallZPos), FRotator(0, 90, 0), FVector((YSize / 2.0f) - TileSize / 2.0f, 5, CeilingHeight / 2.0f));
		}
		else
		{
			AddCollider(FString::Printf(TEXT("Wall_Collider_Right_%d"), wallAdjust), FVector((XSize / 2.0f) - 5, 0, wallZPos), FRotator(0, 90, 0), FVector(YSize / 2.0f, 5, CeilingHeight / 2.0f));
		}
		
	}
}

void ABuilding::AddCollider(FString colliderName, FVector relativeLocation, FRotator relativeRotation,FVector extents)
{
	UBoxComponent* tempCollider = NewObject<UBoxComponent>(this, FName(*colliderName));
	tempCollider->RegisterComponent();
	tempCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	tempCollider->SetCollisionProfileName("BlockAllDynamic");
	tempCollider->SetVisibility(true);
	tempCollider->SetHiddenInGame(false);

	tempCollider->SetRelativeLocation(relativeLocation);
	tempCollider->SetRelativeRotation(relativeRotation);
	tempCollider->SetBoxExtent(extents, false);

	CreatedBoxColliders.Add(tempCollider);
}

