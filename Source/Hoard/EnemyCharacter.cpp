// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "HoardGameModeBase.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	 
	//OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	//OverlapSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	HoardGameMode = Cast<AHoardGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

