// Copyright Epic Games, Inc. All Rights Reserved.


#include "CusSoGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


void ACusSoGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//如果在游戏中
	if (GameState)
	{
		//获取所有玩家
		int32 num = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Cur All Player Num: %d"), num));

		//获取新玩家的状态
		APlayerState* state = NewPlayer->GetPlayerState<APlayerState>();

		//如果获取成功
		if (state)
		{
			//获取新玩家名字
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
