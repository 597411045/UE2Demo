// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Interfaces/OnlineSessionInterface.h"
#include "CusSoSubsystem.generated.h"

namespace EOnJoinSessionCompleteResult
{
	enum Type : int;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCusSoAfterCreateSession, bool, bFlag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCusSoAfterStartSession, bool, bFlag);

DECLARE_MULTICAST_DELEGATE_TwoParams(FCusSoAfterFindSession, const TArray<FOnlineSessionSearchResult>& results,
                                     bool flag);
DECLARE_MULTICAST_DELEGATE_OneParam(FCusSoAfterJoinSession, EOnJoinSessionCompleteResult::Type result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCusSoAfterDestroySession, bool, bFlag);


UCLASS()
class CUSSTEAMONLINE_API UCusSoSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCusSoSubsystem();

	void CreateGameSession();

	void StartGameSession();

	void FindGameSession();

	void JoinGameSession();

	void DestroyGameSession();

	void BeginPlay();

	FCusSoAfterCreateSession DELE_CusSoAfterCreateSession;
	FCusSoAfterStartSession DELE_CusSoAfterStartSession;
	FCusSoAfterFindSession DELE_CusSoAfterFindSession;
	FCusSoAfterJoinSession DELE_CusSoAfterJoinSession;
	FCusSoAfterDestroySession DELE_CusSoAfterDestroySession;


private:
	//IOnlineSessionPtr
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> ioSessionP;
	TSharedPtr<class FOnlineSessionSettings> setting;
	TSharedPtr<class FOnlineSessionSearch> search;

	class FOnlineSessionSearchResult* ValidSession;

	class FDelegateHandle CreateSessionHandle;
	class FDelegateHandle StartSessionHandle;
	class FDelegateHandle FindSessionHandle;
	class FDelegateHandle JoinSessionHandle;
	class FDelegateHandle DestroySessionHandle;

	//FOnCreateSessionComplete
	//TDelegate<void(FName, bool)> afterCreateSession;
	//FOnStartSessionCompleteDelegate
	//TDelegate<void(FName, bool)> afterStartSession;
	//FOnFindSessionsComplete
	//TDelegate<void(bool)> afterFindSession;
	//FOnJoinSessionCompleteDelegate
	//TDelegate<void(FName, EOnJoinSessionCompleteResult::Type)> afterJoinSession;
	//FOnDestroySessionCompleteDelegate
	//TDelegate<void(FName, bool)> afterDestroySession;


	UFUNCTION(BlueprintCallable)
	void OpenLobby();

	UFUNCTION(BlueprintCallable)
	void CallOpenLevel(const FString& address);

	UFUNCTION(BlueprintCallable)
	void CallClientTravel(const FString& address);

	void DoAfterCreateSession(FName name, bool flag);

	void DoAfterStartSession(FName name, bool flag);

	void DoAfterFindSession(bool flag);

	void DoAfterJoinSession(FName name, EOnJoinSessionCompleteResult::Type flag);

	void DoAfterDestroySession(FName name, bool flag);
};
