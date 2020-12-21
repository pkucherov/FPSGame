// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "GASActorBase.h"

// Sets default values
AGASActorBase::AGASActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGASActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

