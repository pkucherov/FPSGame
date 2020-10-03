// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Barricades/ShooterBarricade_Player.h"

// Sets default values
AShooterBarricade_Player::AShooterBarricade_Player()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterBarricade_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterBarricade_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

