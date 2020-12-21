// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GASCharacterMovementComponent.h"

// Sets default values
AGASCharacterMovementComponent::AGASCharacterMovementComponent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGASCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASCharacterMovementComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacterMovementComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

