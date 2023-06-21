// Fill out your copyright notice in the Description page of Project Settings.


#include "Objective.h"

// Sets default values
AObjective::AObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Parent"));
	SetRootComponent(StaticMesh);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AObjective::BeginPlay()
{
	Super::BeginPlay();

	HoardGameMode = Cast<AHoardGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

