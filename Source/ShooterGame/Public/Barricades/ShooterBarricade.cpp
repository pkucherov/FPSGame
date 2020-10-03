// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Barricades/ShooterBarricade.h"

#include "Particles/ParticleSystemComponent.h"

AShooterBarricade::AShooterBarricade(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UCapsuleComponent* CollisionComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CollisionComp"));
	CollisionComp->InitCapsuleSize(40.0f, 50.0f);
	CollisionComp->SetCollisionObjectType(COLLISION_BARRICADE);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;

	BarricadePSC = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("BarricadeFX"));
	BarricadePSC->bAutoActivate = false;
	BarricadePSC->bAutoDestroy = false;
	BarricadePSC->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = false;
	PickedUpBy = NULL;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
}

void AShooterBarricade::BeginPlay()
{
	Super::BeginPlay();

	RespawnBarricade();

	// register on Barricade list (server only), don't care about unregistering (in FinishDestroy) - no streaming
	AShooterGameMode* GameMode = GetWorld()->GetAuthGameMode<AShooterGameMode>();
	if (GameMode)
	{
		GameMode->LevelBarricades.Add(this);
	}
}

void AShooterBarricade::NotifyActorBeginOverlap(class AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	BarricadeOnTouch(Cast<AShooterCharacter>(Other));
}

bool AShooterBarricade::CanBePickedUp(class AShooterCharacter* TestPawn) const
{
	return TestPawn && TestPawn->IsAlive();
}

void AShooterBarricade::GiveBarricadeTo(class AShooterCharacter* Pawn)
{
}

void AShooterBarricade::BarricadeOnTouch(class AShooterCharacter* Pawn)
{
	if (bIsActive && Pawn && Pawn->IsAlive() && !IsPendingKill())
	{
		if (CanBePickedUp(Pawn))
		{
			GiveBarricadeTo(Pawn);
			PickedUpBy = Pawn;

			if (!IsPendingKill())
			{
				bIsActive = false;
				OnPickedUp();

				if (RespawnTime > 0.0f)
				{
					GetWorldTimerManager().SetTimer(TimerHandle_RespawnBarricade, this, &AShooterBarricade::RespawnBarricade, RespawnTime, false);
				}
			}
		}
	}
}

void AShooterBarricade::RespawnBarricade()
{
	bIsActive = true;
	PickedUpBy = NULL;
	OnRespawned();

	TSet<AActor*> OverlappingPawns;
	GetOverlappingActors(OverlappingPawns, AShooterCharacter::StaticClass());

	for (AActor* OverlappingPawn : OverlappingPawns)
	{
		BarricadeOnTouch(CastChecked<AShooterCharacter>(OverlappingPawn));
	}
}

void AShooterBarricade::OnPickedUp()
{
	if (RespawningFX)
	{
		BarricadePSC->SetTemplate(RespawningFX);
		BarricadePSC->ActivateSystem();
	}
	else
	{
		BarricadePSC->DeactivateSystem();
	}

	if (BarricadeSound && PickedUpBy)
	{
		UGameplayStatics::SpawnSoundAttached(BarricadeSound, PickedUpBy->GetRootComponent());
	}

	OnPickedUpEvent();
}

void AShooterBarricade::OnRespawned()
{
	if (ActiveFX)
	{
		BarricadePSC->SetTemplate(ActiveFX);
		BarricadePSC->ActivateSystem();
	}
	else
	{
		BarricadePSC->DeactivateSystem();
	}

	const bool bJustSpawned = CreationTime <= (GetWorld()->GetTimeSeconds() + 5.0f);
	if (RespawnSound && !bJustSpawned)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RespawnSound, GetActorLocation());
	}

	OnRespawnEvent();
}

void AShooterBarricade::OnRep_IsActive()
{
	if (bIsActive)
	{
		OnRespawned();
	}
	else
	{
		OnPickedUp();
	}
}

void AShooterBarricade::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterBarricade, bIsActive);
	DOREPLIFETIME(AShooterBarricade, PickedUpBy);
}
