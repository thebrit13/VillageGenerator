// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "HoardGameModeBase.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnPoints.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SP1")));
	SpawnPoints.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SP2")));
	SpawnPoints.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SP3")));
	SpawnPoints.Add(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SP4")));
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	_HoardGameMode = Cast<AHoardGameModeBase>(GetWorld()->GetAuthGameMode());

	SpawnEnemies(1);
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_EnemiesLeftToSpawn > 0)
	{
		if ((FDateTime::UtcNow() - _LastSpawnTime).GetTotalSeconds() >= _SpawnSeparation)
		{
			SpawnEnemy();
		}
	}
}

void ASpawnManager::SpawnEnemies(int waveCount)
{
	_EnemiesLeftToSpawn = FMath::RandRange(_MinSpawnCount, _MaxSpawnCount);
	_EnemiesLeftToSpawn += (int)(_EnemiesLeftToSpawn * waveCount * _WaveMult);
	_HoardGameMode->SetEnemyCountAndSpawnRef(_EnemiesLeftToSpawn,this);
}

void ASpawnManager::SpawnEnemy()
{
	int spawnNumber = FMath::RandRange(0, 3);
	if (GetWorld()->SpawnActor<AEnemyCharacter>(Enemy1, SpawnPoints[spawnNumber]->GetComponentLocation(), FRotator::ZeroRotator))
	{
		_EnemiesLeftToSpawn--;
		_LastSpawnTime = FDateTime::UtcNow();
	}
}

