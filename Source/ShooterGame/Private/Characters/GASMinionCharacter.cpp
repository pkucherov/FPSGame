// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GASMinionCharacter.h"

// Sets default values
AGASMinionCharacter::AGASMinionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGASMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASMinionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASMinionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

