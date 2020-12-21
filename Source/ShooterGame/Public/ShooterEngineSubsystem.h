// Copyright 2020 Edwin Yung

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "ShooterEngineSubsystem.generated.h"

/**
 * 
 */
//The class name must have an appropiate prefix added (A for Actors, U for other classes
UCLASS()
class SHOOTERGAME_API UShooterEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
