// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "HoardGameModeBase.h"
#include "Objective.generated.h"

UCLASS()
class HOARD_API AObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjective();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereCollision;

	UPROPERTY(BlueprintReadOnly)
	AHoardGameModeBase* HoardGameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
