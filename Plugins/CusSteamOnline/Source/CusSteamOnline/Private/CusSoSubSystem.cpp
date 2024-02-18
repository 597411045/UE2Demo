// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameModeBase.h"

UCusSoSubsystem::UCusSoSubsystem()
{
	BeginPlay();
}

void UCusSoSubsystem::BeginPlay()
{
	// 委托，默认模式
	// afterCreateSession = TDelegate<void(FName, bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterCreateSession);
	// afterStartSession = TDelegate<void(FName, bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterStartSession);
	// afterFindSession = TDelegate<void(bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterFindSession);
	// afterJoinSession = TDelegate<void(FName name, EOnJoinSessionCompleteResult::Type flag)>::CreateUObject(
	// 	this, &UCusSoSubsystem::DoAfterJoinSession);
	// afterDestroySession = TDelegate<void(FName, bool)>::CreateUObject(this, &UCusSoSubsystem::DoAfterDestroySession);
	//

	//初始化网络系统
	IOnlineSubsystem* ioSub = IOnlineSubsystem::Get();
	if (ioSub)
	{
		//获取网络会话
		ioSessionP = ioSub->GetSessionInterface();
		///
		// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		//                                  FString::Printf(
		// 	                                 TEXT("Get A Session Interface %s"),
		// 	                                 *ioSub->GetSubsystemName().ToString()));
	}
}

///Game/Work/Test/Lobby?listen
void UCusSoSubsystem::CallServetTravel(const FString& lobbyName = FString::Printf(TEXT("/Game/Work/Test/Lobby?listen")),
                                       bool bSeam = false)
{
	UWorld* world = GetWorld();
	if (world)
	{
		//使用无缝地图？？
		//world->GetAuthGameMode()->bUseSeamlessTravel = bSeam;

		world->ServerTravel(lobbyName);
	}
}

void UCusSoSubsystem::CallOpenLevel(const FString& address)
{
	UGameplayStatics::OpenLevel(this, *address);
}

void UCusSoSubsystem::CallClientTravel(const FString& address)
{
	//获取本地玩家PC
	APlayerController* PC = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}

void UCusSoSubsystem::CreateGameSession()
{
	//确认网络会话不为空
	if (ioSessionP.IsValid() == false)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Session Interface Is Nulllptr")));
		return;
	}

	//确认网络会话Session为空
	FNamedOnlineSession* sessionName = ioSessionP->GetNamedSession(NAME_GameSession);
	if (sessionName != nullptr)
	{
		ioSessionP->DestroySession(NAME_GameSession);
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
		                                 FString::Printf(TEXT("Destroyed Old Session %d"), NAME_GameSession));
	}
	//初始化设置
	setting = MakeShareable(new FOnlineSessionSettings());
	//如果没有连接steam，就适用局域网
	setting->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	setting->NumPublicConnections = 4;
	setting->bAllowJoinInProgress = true;
	setting->bAllowJoinViaPresence = true;
	setting->bShouldAdvertise = true;
	setting->bUsesPresence = true;
	setting->bUseLobbiesIfAvailable = true;
	//设置传输的值
	setting->Set(FName("TestKey"), FString("TestValue"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	setting->BuildUniqueId = 1;

	//获取本地玩家PC
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();

	//
	// CreateSessionHandle = ioSessionP->AddOnCreateSessionCompleteDelegate_Handle(
	// 	FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCusSoSubsystem::DoAfterCreateSession));

	//绑定委托
	ioSessionP->OnCreateSessionCompleteDelegates.AddUObject(this, &UCusSoSubsystem::DoAfterCreateSession);

	//执行创建Session
	if (ioSessionP->CreateSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *setting) == true)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Try Create Session %d Success"), NAME_GameSession));
	}
	else
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Try Create Session %d Failed"), NAME_GameSession));
		//ioSessionP->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionHandle);
		DELE_CusSoAfterCreateSession.Broadcast(false);

		//ioSessionP->OnCreateSessionCompleteDelegates.Remove(CreateSessionHandle);
		ioSessionP->OnCreateSessionCompleteDelegates.RemoveAll(this);
	}
}

void UCusSoSubsystem::DoAfterCreateSession(FName name, bool flag)
{
	if (flag)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Create Session %s Success"), *name.ToString()));

		CallServetTravel();
	}
	else
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Create Session %s Failed"), *name.ToString()));
	}
	DELE_CusSoAfterCreateSession.Broadcast(flag);
	ioSessionP->OnCreateSessionCompleteDelegates.Remove(CreateSessionHandle);
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
	search->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	if (search->bIsLanQuery == false)
	{
		search->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	}
	//
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	//
	FindSessionHandle = ioSessionP->OnFindSessionsCompleteDelegates.AddUObject(
		this, &UCusSoSubsystem::DoAfterFindSession);
	if (ioSessionP->FindSessions(*player->GetPreferredUniqueNetId(), search.ToSharedRef()) == true)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Try Find Session %d Success"), NAME_GameSession));
	}
	else
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Try Find Session %d Failed"), NAME_GameSession));
		DELE_CusSoAfterFindSession.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		ioSessionP->OnFindSessionsCompleteDelegates.Remove(FindSessionHandle);
	}
}

void UCusSoSubsystem::DoAfterFindSession(bool flag)
{
	///
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
	                                 FString::Printf(TEXT("Find Session Done %d"), NAME_GameSession));
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
		// }

		DELE_CusSoAfterFindSession.Broadcast(search->SearchResults, flag);
		ioSessionP->OnFindSessionsCompleteDelegates.Remove(FindSessionHandle);
	}
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
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("ValidSession Is Nulllptr")));
		return;
	}
	//
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	//
	JoinSessionHandle = ioSessionP->OnJoinSessionCompleteDelegates.AddUObject(
		this, &UCusSoSubsystem::DoAfterJoinSession);
	if (ioSessionP->JoinSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *ValidSession) == true)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Try Join Session %d Success"), NAME_GameSession));
	}
	else
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Try Join Session %d Failed"), NAME_GameSession));
		DELE_CusSoAfterJoinSession.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		ioSessionP->OnJoinSessionCompleteDelegates.Remove(JoinSessionHandle);
	}
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
	ioSessionP->OnJoinSessionCompleteDelegates.Remove(JoinSessionHandle);
	DELE_CusSoAfterJoinSession.Broadcast(flag);
}


void UCusSoSubsystem::DestroyGameSession()
{
}

void UCusSoSubsystem::DoAfterDestroySession(FName name, bool flag)
{
	DELE_CusSoAfterDestroySession.Broadcast(flag);
}
