// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ShooterBarricade.generated.h"

// Base class for Barricade objects that can be placed in the world
UCLASS(abstract)
class AShooterBarricade : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Barricade on touch */
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

	/** check if pawn can use this Barricade */
	virtual bool CanBePickedUp(class AShooterCharacter* TestPawn) const;

protected:
	/** initial setup */
	virtual void BeginPlay() override;

private:
	/** FX component */
	UPROPERTY(VisibleDefaultsOnly, Category=Effects)
	UParticleSystemComponent* BarricadePSC;

protected:
	/** FX of active Barricade */
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	UParticleSystem* ActiveFX;

	/** FX of Barricade on respawn timer */
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	UParticleSystem* RespawningFX;

	/** sound played when player picks it up */
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	USoundCue* BarricadeSound;

	/** sound played on respawn */
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	USoundCue* RespawnSound;

	/** how long it takes to respawn? */
	UPROPERTY(EditDefaultsOnly, Category=Barricade)
	float RespawnTime;

	/** is it ready for interactions? */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_IsActive)
	uint32 bIsActive:1;

	/* The character who has picked up this Barricade */
	UPROPERTY(Transient, Replicated)
	AShooterCharacter* PickedUpBy;

	/** Handle for efficient management of RespawnBarricade timer */
	FTimerHandle TimerHandle_RespawnBarricade;

	UFUNCTION()
	void OnRep_IsActive();

	/** give Barricade */
	virtual void GiveBarricadeTo(class AShooterCharacter* Pawn);

	/** handle touches */
	void BarricadeOnTouch(class AShooterCharacter* Pawn);

	/** show and enable Barricade */
	virtual void RespawnBarricade();

	/** show effects when Barricade disappears */
	virtual void OnPickedUp();

	/** show effects when Barricade appears */
	virtual void OnRespawned();

	/** blueprint event: Barricade disappears */
	UFUNCTION(BlueprintImplementableEvent)
	void OnPickedUpEvent();

	/** blueprint event: Barricade appears */
	UFUNCTION(BlueprintImplementableEvent)
	void OnRespawnEvent();

protected:
	/** Returns BarricadePSC subobject **/
	FORCEINLINE UParticleSystemComponent* GetBarricadePSC() const { return BarricadePSC; }
};
