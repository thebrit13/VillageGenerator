// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyCharacter.h"
#include "SpawnManager.generated.h"

UCLASS()
class HOARD_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
	TArray<UStaticMeshComponent*> SpawnPoints;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyCharacter> Enemy1;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemies(int waveCount);

	void SpawnEnemy();

private:

	int _MinSpawnCount = 5;
	int _MaxSpawnCount = 10;

	int _EnemiesLeftToSpawn = 0;

	float _SpawnSeparation = .25f;

	float _WaveMult = .2f;

	FDateTime _LastSpawnTime = FDateTime::MinValue();

	class AHoardGameModeBase* _HoardGameMode;

};
