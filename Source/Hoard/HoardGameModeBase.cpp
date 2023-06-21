// Copyright Epic Games, Inc. All Rights Reserved.


#include "HoardGameModeBase.h"

void AHoardGameModeBase::UpdateObjectiveHealth(int amount)
{
	ObjectiveHealth += amount;
	UpdateUI();
}

void AHoardGameModeBase::EnemyKilled()
{
	_CurrentEnemyCount--;

	if (_CurrentEnemyCount <= 0)
	{
		_WaveCount++;
		if (_WaveCount <= _MaxWaveCount)
		{
			if (_SpawnManager)
			{
				_SpawnManager->SpawnEnemies(_WaveCount);
			}
		}
		else
		{
			//game over
		}
	}
	UpdateUI();
}

void AHoardGameModeBase::SetEnemyCountAndSpawnRef(int Amt, ASpawnManager* spawnManager)
{
	_CurrentEnemyCount = Amt;
	_SpawnManager = spawnManager;
}

