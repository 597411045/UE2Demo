// Copyright Epic Games, Inc. All Rights Reserved.


#include "CusSoGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


void ACusSoGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState)
	{
		int32 num = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Cur All Player Num: %d"), num));

		APlayerState* state = NewPlayer->GetPlayerState<APlayerState>();
		if (state)
		{
			FString name = state->GetPlayerName();
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue,
			                                 FString::Printf(TEXT("Join Player Name: %s"), *name));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, FString::Printf(TEXT("NoGameState")));
		}
	}
}

void ACusSoGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (GameState)
	{
		int32 num = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow,
		                                 FString::Printf(TEXT("Cur All Player Num: %d"), num));

		APlayerState* state = Exiting->GetPlayerState<APlayerState>();
		if (state)
		{
			FString name = state->GetPlayerName();
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue,
			                                 FString::Printf(TEXT("Exit Player Name: %s"), *name));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, FString::Printf(TEXT("NoGameState")));
	}
}
