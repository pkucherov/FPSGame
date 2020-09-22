// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ShooterBarricade.h"
#include "ShooterBarricade_Health.generated.h"

class AShooterCharacter;

// A barricade object that replenishes character health
UCLASS(Abstract, Blueprintable)
class AShooterBarricade_Health : public AShooterBarricade
{
	GENERATED_UCLASS_BODY()

	/** check if pawn can use this barricade */
	virtual bool CanBePickedUp(AShooterCharacter* TestPawn) const override;

protected:

	/** how much health does it give? */
	UPROPERTY(EditDefaultsOnly, Category=Barricade)
	int32 Health;

	/** give pickup */
	virtual void GiveBarricadeTo(AShooterCharacter* Pawn) override;
};
