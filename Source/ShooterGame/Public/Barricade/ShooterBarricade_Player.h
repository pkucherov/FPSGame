// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ShooterBarricade.h"
#include "ShooterBarricade_Ammo.generated.h"

class AShooterCharacter;
class AShooterWeapon;

// A pickup object that replenishes ammunition for a weapon
UCLASS(Abstract, Blueprintable)
class AShooterBarricade_Ammo : public AShooterBarricade
{
	GENERATED_UCLASS_BODY()

	/** check if pawn can use this pickup */
	virtual bool CanBePickedUp(AShooterCharacter* TestPawn) const override;

	bool IsForWeapon(UClass* WeaponClass);

protected:

	/** how much ammo does it give? */
	UPROPERTY(EditDefaultsOnly, Category=Barricade)
	int32 AmmoClips;

	/** which weapon gets ammo? */
	UPROPERTY(EditDefaultsOnly, Category=Barricade)
	TSubclassOf<AShooterWeapon> WeaponType;

	/** give pickup */
	virtual void GiveBarricadeTo(AShooterCharacter* Pawn) override;
};
