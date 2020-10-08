// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShooterBarricade.h"
#include "ShooterBarricade_Player.generated.h"


class AShooterCharacter;
class AShooterWeapon;

// A Barricade object that replenishes ammunition for a weapon
UCLASS(Abstract, Blueprintable)
class AShooterBarricade_Ammo : public AShooterBarricade
{
	GENERATED_UCLASS_BODY()

	/** check if pawn can use this Barricade */
	virtual bool CanBePickedUp(AShooterCharacter* TestPawn) const override;

	bool IsForWeapon(UClass* WeaponClass);

protected:

	/** how much ammo does it give? */
	UPROPERTY(EditDefaultsOnly, Category = Barricade)
		int32 AmmoClips;

	/** which weapon gets ammo? */
	UPROPERTY(EditDefaultsOnly, Category = Barricade)
		TSubclassOf<AShooterWeapon> WeaponType;

	/** give Barricade */
	virtual void GiveBarricadeTo(AShooterCharacter* Pawn) override;
};
