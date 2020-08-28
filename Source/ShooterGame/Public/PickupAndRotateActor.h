// Copyright. All Rights Reserved

#pragma once
#include "GameFramework/Actor.h"
#include "PickupAndRotateActor.generated.h"

class UCameraComponent;
class ACharacter;

UCLASS()
class SHOOTERGAME_API APickupAndRotateActor : public AActor
{
	GENERATED_BODY()

public:
	//void AMyActor(const FObjectInitializer& ObjectInitializer);
	// Sets default values for this actor's properties
	APickupAndRotateActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* HoldingComp;

	UFUNCTION()
		void RotateActor();

	UFUNCTION()
		void Pickup();

	bool bHolding;
	bool bGravity;

	FRotator ControlRotation;
	ACharacter* MyCharacter;
	UCameraComponent* PlayerCamera;
	FVector ForwardVector;
};
