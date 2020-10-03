// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Barricades/ShooterBarricade_Environment.h"

// Sets default values
AShooterBarricade_Environment::AShooterBarricade_Environment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterBarricade_Environment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterBarricade_Environment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

