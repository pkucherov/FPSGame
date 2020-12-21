// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GASHeroCharacter.h"

// Sets default values
AGASHeroCharacter::AGASHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGASHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

