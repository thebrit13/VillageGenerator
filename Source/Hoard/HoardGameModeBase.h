// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnManager.h"
#include "HoardGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HOARD_API AHoardGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int ObjectiveHealth;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int StartingObjectiveHealth;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUI();
public:

	UFUNCTION(BlueprintCallable)
	void UpdateObjectiveHealth(int amount);

	UFUNCTION(BlueprintCallable)
	void EnemyKilled();

	void SetEnemyCountAndSpawnRef(int Amt, ASpawnManager* spawnManager);

private:

	int _WaveCount = 1;

	int _MaxWaveCount = 5;

	int _CurrentEnemyCount = 0;

	ASpawnManager* _SpawnManager;
};
