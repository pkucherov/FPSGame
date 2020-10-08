// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "ShooterTeamStart.h"
#include "Online/ShooterGame_TeamAssimilation.h"
#include "Online/ShooterPlayerState.h"
#include "Bots/ShooterAIController.h"

AShooterGame_TeamAssimilation::AShooterGame_TeamAssimilation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NumTeams = 2;
	bDelayedStart = true;
}

void AShooterGame_TeamAssimilation::PostLogin(APlayerController* NewPlayer)
{
	// Place player on a team before Super (VoIP team based init, findplayerstart, etc)
	AShooterPlayerState* NewPlayerState = CastChecked<AShooterPlayerState>(NewPlayer->PlayerState);
	const int32 TeamNum = ChooseTeam(NewPlayerState);
	NewPlayerState->SetTeamNum(TeamNum);

	Super::PostLogin(NewPlayer);
}

void AShooterGame_TeamAssimilation::InitGameState()
{
	Super::InitGameState();

	AShooterGameState* const MyGameState = Cast<AShooterGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->NumTeams = NumTeams;
	}
}

bool AShooterGame_TeamAssimilation::CanDealDamage(AShooterPlayerState* DamageInstigator, class AShooterPlayerState* DamagedPlayer) const
{
	return DamageInstigator && DamagedPlayer && (DamagedPlayer == DamageInstigator || DamagedPlayer->GetTeamNum() != DamageInstigator->GetTeamNum());
}

int32 AShooterGame_TeamAssimilation::ChooseTeam(AShooterPlayerState* ForPlayerState) const
{
	TArray<int32> TeamBalance;
	TeamBalance.AddZeroed(NumTeams);

	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++) 
	{
		AShooterPlayerState const* const TestPlayerState = Cast<AShooterPlayerState>(GameState->PlayerArray[i]);
		if (TestPlayerState && TestPlayerState != ForPlayerState && TeamBalance.IsValidIndex(TestPlayerState->GetTeamNum()))
		{
			TeamBalance[TestPlayerState->GetTeamNum()]++;
		}
	}

	// 27 survivors vs 3
	//Team 0 are Survivors
	//Team 1 is the Collective
	// get current team balance
	// find least populated one
	int32 SurvivorScore = TeamBalance[0];
	int32 CollectiveScore = TeamBalance[1];
	if (CollectiveScore < 3)
	{
		return 1;
	}

	return 0;

	//for (int32 i = 1; i < TeamBalance.Num(); i++)
	//{
	//	if (BestTeamScore > TeamBalance[i])
	//	{
	//		BestTeamScore = TeamBalance[i];
	//	}
	//}

	//// there could be more than one...
	//TArray<int32> BestTeams;
	//for (int32 i = 0; i < TeamBalance.Num(); i++)
	//{
	//	if (TeamBalance[i] == BestTeamScore)
	//	{
	//		BestTeams.Add(i);
	//	}
	//}

	//// get random from best list
	//const int32 RandomBestTeam = BestTeams[FMath::RandHelper(BestTeams.Num())];
	//return RandomBestTeam;
}

void AShooterGame_TeamAssimilation::DetermineMatchWinner()
{
	AShooterGameState const* const MyGameState = Cast<AShooterGameState>(GameState);
	int32 BestScore = MIN_uint32;
	int32 BestTeam = -1;
	int32 NumBestTeams = 1;

	for (int32 i = 0; i < MyGameState->TeamScores.Num(); i++)
	{
		const int32 TeamScore = MyGameState->TeamScores[i];
		if (BestScore < TeamScore)
		{
			BestScore = TeamScore;
			BestTeam = i;
			NumBestTeams = 1;
		}
		else if (BestScore == TeamScore)
		{
			NumBestTeams++;
		}
	}

	WinnerTeam = (NumBestTeams == 1) ? BestTeam : NumTeams;
}

bool AShooterGame_TeamAssimilation::IsWinner(AShooterPlayerState* PlayerState) const
{
	return PlayerState && !PlayerState->IsQuitter() && PlayerState->GetTeamNum() == WinnerTeam;
}

bool AShooterGame_TeamAssimilation::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const
{
	if (Player)
	{
		AShooterTeamStart* TeamStart = Cast<AShooterTeamStart>(SpawnPoint);
		AShooterPlayerState* PlayerState = Cast<AShooterPlayerState>(Player->PlayerState);

		if (PlayerState && TeamStart && TeamStart->SpawnTeam != PlayerState->GetTeamNum())
		{
			return false;
		}
	}

	return Super::IsSpawnpointAllowed(SpawnPoint, Player);
}

void AShooterGame_TeamAssimilation::InitBot(AShooterAIController* AIC, int32 BotNum)
{	
	AShooterPlayerState* BotPlayerState = CastChecked<AShooterPlayerState>(AIC->PlayerState);
	const int32 TeamNum = ChooseTeam(BotPlayerState);
	BotPlayerState->SetTeamNum(TeamNum);		

	Super::InitBot(AIC, BotNum);
}