// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterBarricade.h"
#include "ShooterBarricade_Environment.generated.h"


class AShooterCharacter;

// A Barricade object that replenishes character health
UCLASS(Abstract, Blueprintable)
class AShooterBarricade_Health : public AShooterBarricade
{
	GENERATED_UCLASS_BODY()

		/** check if pawn can use this Barricade */
		virtual bool CanBePickedUp(AShooterCharacter* TestPawn) const override;

protected:

	/** how much health does it give? */
	UPROPERTY(EditDefaultsOnly, Category = Barricade)
		int32 Health;

	/** give Barricade */
	virtual void GiveBarricadeTo(AShooterCharacter* Pawn) override;
};
