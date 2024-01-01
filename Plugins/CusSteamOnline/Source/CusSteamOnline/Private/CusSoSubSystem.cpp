// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

UCusSoSubsystem::UCusSoSubsystem()
{
	BeginPlay();
}

void UCusSoSubsystem::BeginPlay()
{
	// ...
	//
	// afterCreateSession = TDelegate<void(FName, bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterCreateSession);
	// afterStartSession = TDelegate<void(FName, bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterStartSession);
	// afterFindSession = TDelegate<void(bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterFindSession);
	// afterJoinSession = TDelegate<void(FName name, EOnJoinSessionCompleteResult::Type flag)>::CreateUObject(
	// 	this, &UCusSoSubsystem::DoAfterJoinSession);
	// afterDestroySession = TDelegate<void(FName, bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterDestroySession);
	//
	IOnlineSubsystem* ioSub = IOnlineSubsystem::Get();
	if (ioSub)
	{
		ioSessionP = ioSub->GetSessionInterface();
		///
		// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		//                                  FString::Printf(
		// 	                                 TEXT("Get A Session Interface %s"),
		// 	                                 *ioSub->GetSubsystemName().ToString()));
	}
}


void UCusSoSubsystem::OpenLobby()
{
	UWorld* world = GetWorld();
	if (world)
	{
		world->ServerTravel("/Game/Work/Test/Lobby?listen");
	}
}

void UCusSoSubsystem::CallOpenLevel(const FString& address)
{
	UGameplayStatics::OpenLevel(this, *address);
}

void UCusSoSubsystem::CallClientTravel(const FString& address)
{
	APlayerController* PC = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}

void UCusSoSubsystem::CreateGameSession()
{
	if (ioSessionP.IsValid() == false)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Session Interface Is Nulllptr")));
		return;
	}
	FNamedOnlineSession* sessionName = ioSessionP->GetNamedSession(NAME_GameSession);
	if (sessionName != nullptr)
	{
		ioSessionP->DestroySession(NAME_GameSession);
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
		                                 FString::Printf(TEXT("Destroyed Old Session %d"), NAME_GameSession));
	}
	//
	setting = MakeShareable(new FOnlineSessionSettings());
	setting->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	setting->NumPublicConnections = 4;
	setting->bAllowJoinInProgress = true;
	setting->bAllowJoinViaPresence = true;
	setting->bShouldAdvertise = true;
	setting->bUsesPresence = true;
	setting->bUseLobbiesIfAvailable = true;
	setting->Set(FName("TestKey"), FString("TestValue"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	//
	// CreateSessionHandle = ioSessionP->AddOnCreateSessionCompleteDelegate_Handle(
	// 	FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCusSoSubsystem::DoAfterCreateSession));

	ioSessionP->OnCreateSessionCompleteDelegates.AddUObject(this, &UCusSoSubsystem::DoAfterCreateSession);
	if (ioSessionP->CreateSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *setting) == true)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
		                                 FString::Printf(TEXT("Try Create Session %d Success"), NAME_GameSession));
	}
	else
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
		                                 FString::Printf(TEXT("Try Create Session %d Failed"), NAME_GameSession));
		//ioSessionP->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
		ioSessionP->OnCreateSessionCompleteDelegates.Remove(CreateSessionHandle);
	}
}

void UCusSoSubsystem::DoAfterCreateSession(FName name, bool flag)
{
	if (flag)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Create Session %s Success"), *name.ToString()));

		OpenLobby();
	}
	else
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Create Session %s Failed"), *name.ToString()));
	}
	DELE_CusSoAfterCreateSession.Broadcast(flag);
	ioSessionP->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
}

void UCusSoSubsystem::StartGameSession()
{
}

void UCusSoSubsystem::DoAfterStartSession(FName name, bool flag)
{
	DELE_CusSoAfterStartSession.Broadcast(flag);
}

void UCusSoSubsystem::FindGameSession()
{
	if (ioSessionP.IsValid() == false)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Session Interface Is Nulllptr")));
		return;
	}
	//
	search = MakeShareable(new FOnlineSessionSearch());
	search->MaxSearchResults = 10000;
	search->bIsLanQuery = false;
	search->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	//
	ioSessionP->OnFindSessionsCompleteDelegates.AddUObject(this, &UCusSoSubsystem::DoAfterFindSession);
	ioSessionP->FindSessions(*player->GetPreferredUniqueNetId(), search.ToSharedRef());
	///
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
	                                 FString::Printf(TEXT("Finding A Session %d"), NAME_GameSession));
}

void UCusSoSubsystem::DoAfterFindSession(bool flag)
{
	///
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
	                                 FString::Printf(TEXT("Finding Session Done %d"), NAME_GameSession));
	//
	//for (FOnlineSessionSearchResult result : search->SearchResults)
	for (int i = 0; i < search->SearchResults.Num(); i++)
	{
		FString id = search->SearchResults[i].GetSessionIdStr();
		FString user = search->SearchResults[i].Session.OwningUserName;
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
		                                 FString::Printf(TEXT("Get A ID: %s, User: %s"), *id, *user));
		//
		FString testKey;
		search->SearchResults[i].Session.SessionSettings.Get(FName("TestKey"), testKey);
		if (testKey == FString("TestValue"))
		{
			ValidSession = &search->SearchResults[i];
			///
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
			                                 FString::Printf(TEXT("This Is A Valid Session")));
			break;
		}
	}

	DELE_CusSoAfterFindSession.Broadcast(search->SearchResults, flag);
}

void UCusSoSubsystem::JoinGameSession()
{
	if (ioSessionP.IsValid() == false)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Session Interface Is Nulllptr")));
		return;
	}


	if (ValidSession == nullptr)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("ValidSession Is Nulllptr")));
		return;
	}
	//
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	//
	ioSessionP->OnJoinSessionCompleteDelegates.AddUObject(this, &UCusSoSubsystem::DoAfterJoinSession);
	ioSessionP->JoinSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *ValidSession);
}

void UCusSoSubsystem::DoAfterJoinSession(FName name, EOnJoinSessionCompleteResult::Type flag)
{
	FString Address;
	if (ioSessionP->GetResolvedConnectString(NAME_GameSession, Address))
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("GetResolvedConnectString Success: %s"), *Address));
		//
		CallClientTravel(Address);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("GetResolvedConnectString Failed")));
	}
	DELE_CusSoAfterJoinSession.Broadcast(flag);
}


void UCusSoSubsystem::DestroyGameSession()
{
}

void UCusSoSubsystem::DoAfterDestroySession(FName name, bool flag)
{
	DELE_CusSoAfterDestroySession.Broadcast(flag);
}
