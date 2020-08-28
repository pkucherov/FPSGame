// Copyright. All Rights Reserved
// Helpful Links
// http://api.unrealengine.com/INT/API/Runtime/Engine/Engine/UWorld/LineTraceSingleByChannel/index.html
// http://api.unrealengine.com/INT/API/Runtime/Engine/FCollisionQueryParams/index.html
// http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/APawn/index.html

#include "PickupAndRotateActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
APickupAndRotateActor::APickupAndRotateActor()//(const FObjectInitializer& ObjectInitializer)
	//: Super(ObjectInitializer)
{
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	MyMesh->SetSimulatePhysics(true);
	RootComponent = MyMesh;

	bHolding = false;
	bGravity = true;
}

// Called when the game starts or when spawned
void APickupAndRotateActor::BeginPlay()
{
	Super::BeginPlay();
	AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(GetWorld()->GetFirstPlayerController());
	//MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	//MyPC = GetWorld()->GetFirstPlayerController();
	PlayerCamera = MyPC->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;

	MyPC->GetComponents(Components);

	if (Components.Num() > 0)
	{
		for (auto& Comp : Components)
		{
			if (Comp->GetName() == "HoldingComponent")
			{
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}
	}
}

void APickupAndRotateActor::RotateActor()
{
	ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SetActorRotation(FQuat(ControlRotation));
}

void APickupAndRotateActor::Pickup()
{
	bHolding = !bHolding;
	bGravity = !bGravity;
	MyMesh->SetEnableGravity(bGravity);
	MyMesh->SetSimulatePhysics(bHolding ? false : true);
	MyMesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	if (HoldingComp && bHolding)
	{
		MyMesh->AttachToComponent(HoldingComp, FAttachmentTransformRules::KeepWorldTransform);
		SetActorLocation(HoldingComp->GetComponentLocation());
	}

	if (!bHolding)
	{
		MyMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ForwardVector = PlayerCamera->GetForwardVector();
		MyMesh->AddForce(ForwardVector * 100000 * MyMesh->GetMass());
	}
}