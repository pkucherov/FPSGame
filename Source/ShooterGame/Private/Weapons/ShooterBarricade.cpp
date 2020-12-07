// Copyright Edwin Yung. All Rights Reserved.


#include "ShooterGame.h"
#include "Weapons/ShooterBarricade.h"
#include "Particles/ParticleSystemComponent.h"
#include "Effects/ShooterExplosionEffect.h"

AShooterBarricade::AShooterBarricade(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(COLLISION_BARRICADE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = CollisionComp;

	ParticleComp = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = false;
	ParticleComp->bAutoDestroy = false;
	ParticleComp->SetupAttachment(RootComponent);

	MovementComp = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	SetReplicatingMovement(true);

	bExploded = true;
}

void AShooterBarricade::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MovementComp->OnProjectileStop.AddDynamic(this, &AShooterBarricade::OnImpact);
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());

	AShooterWeapon_Projectile* OwnerWeapon = Cast<AShooterWeapon_Projectile>(GetOwner());
	if (OwnerWeapon)
	{
		OwnerWeapon->ApplyWeaponConfig(WeaponConfig);
	}

	//Otherwise barricade disappears after this lifespan
//	SetLifeSpan(WeaponConfig.ProjectileLife);
	MyController = GetInstigatorController();
}

void AShooterBarricade::InitVelocity(FVector& ShootDirection)
{
	if (MovementComp)
	{
		MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	}
}

void AShooterBarricade::OnImpact(const FHitResult& HitResult)
{
	if (GetLocalRole() == ROLE_Authority && !bExploded)
	{
		Explode(HitResult);
		Disable();
	}
}

void AShooterBarricade::Explode(const FHitResult& Impact)
{
	if (ParticleComp)
	{
		ParticleComp->Deactivate();
	}

	//// effects and damage origin shouldn't be placed inside mesh at impact point
	const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;

	//if (WeaponConfig.ExplosionDamage > 0 && WeaponConfig.ExplosionRadius > 0 && WeaponConfig.DamageType)
	//{
	//	UGameplayStatics::ApplyRadialDamage(this, WeaponConfig.ExplosionDamage, NudgedImpactLocation, WeaponConfig.ExplosionRadius, WeaponConfig.DamageType, TArray<AActor*>(), this, MyController.Get());
	//}


	//Play FX upon placement of Barricade
	if (ExplosionTemplate)
	{
		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), NudgedImpactLocation);
		AShooterExplosionEffect* const EffectActor = GetWorld()->SpawnActorDeferred<AShooterExplosionEffect>(ExplosionTemplate, SpawnTransform);
		if (EffectActor)
		{
			EffectActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
		}
	}

	bExploded = true;
}

void AShooterBarricade::Disable()
{
	UAudioComponent* ProjAudioComp = FindComponentByClass<UAudioComponent>();
	if (ProjAudioComp && ProjAudioComp->IsPlaying())
	{
		ProjAudioComp->FadeOut(0.1f, 0.f);
	}

	MovementComp->StopMovementImmediately();

	// give clients some time to show explosion
	SetLifeSpan(1.0f);
}

///CODE_SNIPPET_START: AActor::GetActorLocation AActor::GetActorRotation
void AShooterBarricade::OnRep_Exploded()
{
	//FVector ProjDirection = GetActorForwardVector();

	//const FVector StartTrace = GetActorLocation() - ProjDirection * 200;
	//const FVector EndTrace = GetActorLocation() + ProjDirection * 150;
	//FHitResult Impact;

	//if (!GetWorld()->LineTraceSingleByChannel(Impact, StartTrace, EndTrace, COLLISION_PROJECTILE, FCollisionQueryParams(SCENE_QUERY_STAT(ProjClient), true, GetInstigator())))
	//{
	//	// failsafe
	//	Impact.ImpactPoint = GetActorLocation();
	//	Impact.ImpactNormal = -ProjDirection;
	//}

	//Explode(Impact);
}
///CODE_SNIPPET_END

bool AShooterBarricade::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	//if (bIsDying										// already dying
	//	|| IsPendingKill()								// already destroyed
	//	|| GetLocalRole() != ROLE_Authority				// not authority
	//	|| GetWorld()->GetAuthGameMode<AShooterGameMode>() == NULL
	//	|| GetWorld()->GetAuthGameMode<AShooterGameMode>()->GetMatchState() == MatchState::LeavingMap)	// level transition occurring
	//{
	//	return false;
	//}

	return true;
}

bool AShooterBarricade::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	{
		return false;
	}

	Health = FMath::Min(0.0f, Health);

	// if this is an environmental death then refer to the previous killer so that they receive credit (knocked into lava pits, etc)
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	//Killer = GetDamageInstigator(Killer, *DamageType);

	//AController* const KilledPlayer = (Controller != NULL) ? Controller : Cast<AController>(GetOwner());
	//GetWorld()->GetAuthGameMode<AShooterGameMode>()->Killed(Killer, KilledPlayer, this, DamageType);

	//NetUpdateFrequency = GetDefault<AShooterCharacter>()->NetUpdateFrequency;
	//GetCharacterMovement()->ForceReplicationUpdate();

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);

	return true;
}

void AShooterBarricade::KilledBy(APawn* EventInstigator)
{
/*	if (GetLocalRole() == ROLE_Authority && !bIsDying)
	{
		AController* Killer = NULL;
		if (EventInstigator != NULL)
		{
			Killer = EventInstigator->Controller;
			LastHitBy = NULL;
		}

		Die(Health, FDamageEvent(UDamageType::StaticClass()), Killer, NULL);
	} */
}

void AShooterBarricade::ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser, bool bKilled)
{
	const float TimeoutTime = GetWorld()->GetTimeSeconds() + 0.5f;

	FDamageEvent const& LastDamageEvent = LastTakeHitInfo.GetDamageEvent();
	if ((PawnInstigator == LastTakeHitInfo.PawnInstigator.Get()) && (LastDamageEvent.DamageTypeClass == LastTakeHitInfo.DamageTypeClass) && (LastTakeHitTimeTimeout == TimeoutTime))
	{
		// same frame damage
		if (bKilled && LastTakeHitInfo.bKilled)
		{
			// Redundant death take hit, just ignore it
			return;
		}

		// otherwise, accumulate damage done this frame
		Damage += LastTakeHitInfo.ActualDamage;
	}

	LastTakeHitInfo.ActualDamage = Damage;
	LastTakeHitInfo.PawnInstigator = Cast<AShooterCharacter>(PawnInstigator);
	LastTakeHitInfo.DamageCauser = DamageCauser;
	LastTakeHitInfo.SetDamageEvent(DamageEvent);
	LastTakeHitInfo.bKilled = bKilled;
	LastTakeHitInfo.EnsureReplication();

	LastTakeHitTimeTimeout = TimeoutTime;
}

void AShooterBarricade::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ReplicateHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, false);

	//	// play the force feedback effect on the client player controller
	//	AShooterPlayerController* PC = Cast<AShooterPlayerController>(Controller);
	//	if (PC && DamageEvent.DamageTypeClass)
	//	{
	//		UShooterDamageType* DamageType = Cast<UShooterDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
	//		if (DamageType && DamageType->HitForceFeedback && PC->IsVibrationEnabled())
	//		{
	//			FForceFeedbackParameters FFParams;
	//			FFParams.Tag = "Damage";
	//			PC->ClientPlayForceFeedback(DamageType->HitForceFeedback, FFParams);
	//		}
	//	}
	//}

	//if (DamageTaken > 0.f)
	//{
	//	ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	//}

	//AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(Controller);
	//AShooterHUD* MyHUD = MyPC ? Cast<AShooterHUD>(MyPC->GetHUD()) : NULL;
	//if (MyHUD)
	//{
	//	MyHUD->NotifyWeaponHit(DamageTaken, DamageEvent, PawnInstigator);
	//}

	//if (PawnInstigator && PawnInstigator != this && PawnInstigator->IsLocallyControlled())
	//{
	//	AShooterPlayerController* InstigatorPC = Cast<AShooterPlayerController>(PawnInstigator->Controller);
	//	AShooterHUD* InstigatorHUD = InstigatorPC ? Cast<AShooterHUD>(InstigatorPC->GetHUD()) : NULL;
	//	if (InstigatorHUD)
	//	{
	//		InstigatorHUD->NotifyEnemyHit();
	//	}
	}
}

float AShooterBarricade::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	//AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(Controller);
	//if (MyPC && MyPC->HasGodMode())
	//{
	//	return 0.f;
	//}

	if (Health <= 0.f)
	{
		return 0.f;
	}

	//// Modify based on game rules.
	//AShooterGameMode* const Game = GetWorld()->GetAuthGameMode<AShooterGameMode>();
	//Damage = Game ? Game->ModifyDamage(Damage, this, DamageEvent, EventInstigator, DamageCauser) : 0.f;

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			PlayHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}

		//MakeNoise(1.0f, EventInstigator ? EventInstigator->GetPawn() : this);
	}

	return ActualDamage;
}



void AShooterBarricade::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	if (MovementComp)
	{
		MovementComp->Velocity = NewVelocity;
	}
}

void AShooterBarricade::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterBarricade, bExploded);
}



void AShooterBarricade::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}

	SetReplicatingMovement(false);
	TearOff();
	bIsDying = true;

	if (GetLocalRole() == ROLE_Authority)
	{
		ReplicateHit(KillingDamage, DamageEvent, PawnInstigator, DamageCauser, true);

		//Now Destroy the Barricade given its "death"
		//TODO: Put EXPLODE FX here as a placeholder

		//FPointDamageEvent const* const PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);dd

		UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
			Explode(PointDamageEvent->HitInfo);
		}
		else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
		{
			FRadialDamageEvent* const RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
			//if (ActualDamage != 0.f)
			//{
			FHitResult const& Hit = (RadialDamageEvent->ComponentHits.Num() > 0) ? RadialDamageEvent->ComponentHits[0] : FHitResult();
			Explode(Hit);
		}
		Disable();
		

	//	// play the force feedback effect on the client player controller
	//	AShooterPlayerController* PC = Cast<AShooterPlayerController>(Controller);
	//	if (PC && DamageEvent.DamageTypeClass)
	//	{
	//		UShooterDamageType* DamageType = Cast<UShooterDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
	//		if (DamageType && DamageType->KilledForceFeedback && PC->IsVibrationEnabled())
	//		{
	//			FForceFeedbackParameters FFParams;
	//			FFParams.Tag = "Damage";
	//			PC->ClientPlayForceFeedback(DamageType->KilledForceFeedback, FFParams);
	//		}
	//	}
	}

	//// cannot use IsLocallyControlled here, because even local client's controller may be NULL here
	//if (GetNetMode() != NM_DedicatedServer && DeathSound && Mesh1P && Mesh1P->IsVisible())
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	//}

	//// remove all weapons
	//DestroyInventory();

	//// switch back to 3rd person view
	//UpdatePawnMeshes();

	//DetachFromControllerPendingDestroy();
	//StopAllAnimMontages();

	//if (LowHealthWarningPlayer && LowHealthWarningPlayer->IsPlaying())
	//{
	//	LowHealthWarningPlayer->Stop();
	//}

	//if (RunLoopAC)
	//{
	//	RunLoopAC->Stop();
	//}

	//if (GetMesh())
	//{
	//	static FName CollisionProfileName(TEXT("Ragdoll"));
	//	GetMesh()->SetCollisionProfileName(CollisionProfileName);
	//}
	//SetActorEnableCollision(true);

	//// Death anim
	//float DeathAnimDuration = PlayAnimMontage(DeathAnim);

	//// Ragdoll
	//if (DeathAnimDuration > 0.f)
	//{
	//	// Trigger ragdoll a little before the animation early so the character doesn't
	//	// blend back to its normal position.
	//	const float TriggerRagdollTime = DeathAnimDuration - 0.7f;

	//	// Enable blend physics so the bones are properly blending against the montage.
	//	GetMesh()->bBlendPhysics = true;

	//	// Use a local timer handle as we don't need to store it for later but we don't need to look for something to clear
	//	FTimerHandle TimerHandle;
	//	GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterCharacter::SetRagdollPhysics, FMath::Max(0.1f, TriggerRagdollTime), false);
	//}
	//else
	//{
	//	SetRagdollPhysics();
	//}

	//// disable collisions on capsule
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AShooterBarricade::OnRep_LastTakeHitInfo()
{
	if (LastTakeHitInfo.bKilled)
	{
		OnDeath(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
	else
	{
		PlayHit(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
}
