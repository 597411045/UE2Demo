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
void UCusSoSubsystem::CallServetTravel(const FString& lobbyName, bool bSeam = false)
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

	//委托，单一函数模式
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
		//Session创建成功的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Create Session %s Success"), *name.ToString()));

		CallServetTravel(FString::Printf(TEXT("/Game/Work/Test/Lobby?listen")), false);
	}
	else
	{
		//Session创建失败的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Create Session %s Failed"), *name.ToString()));
	}
	//调用注册进来的委托
	DELE_CusSoAfterCreateSession.Broadcast(flag);
	//移除Subsys里的所有委托
	//ioSessionP->OnCreateSessionCompleteDelegates.Remove(CreateSessionHandle);
	ioSessionP->OnCreateSessionCompleteDelegates.RemoveAll(this);
}

void UCusSoSubsystem::StartGameSession()
{
	//暂未使用
}

void UCusSoSubsystem::DoAfterStartSession(FName name, bool flag)
{
	DELE_CusSoAfterStartSession.Broadcast(flag);
}

void UCusSoSubsystem::FindGameSession()
{
	if (ioSessionP.IsValid() == false)
	{
		//如果网络会话接口为空，return
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Session Interface Is Nulllptr")));
		return;
	}
	//设置网络会话搜索设置
	search = MakeShareable(new FOnlineSessionSearch());
	search->MaxSearchResults = 10000;
	search->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	if (search->bIsLanQuery == false)
	{
		search->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	}
	//获取本地玩家
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	//
	/*FindSessionHandle = ioSessionP->OnFindSessionsCompleteDelegates.AddUObject(
		this, &UCusSoSubsystem::DoAfterFindSession);*/
	ioSessionP->OnFindSessionsCompleteDelegates.AddUObject(this, &UCusSoSubsystem::DoAfterFindSession);

	if (ioSessionP->FindSessions(*player->GetPreferredUniqueNetId(), search.ToSharedRef()) == true)
	{
		//找到了Session的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Try Find Session %d Success"), NAME_GameSession));
	}
	else
	{
		//没找到Session的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Try Find Session %d Failed"), NAME_GameSession));
		DELE_CusSoAfterFindSession.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		//ioSessionP->OnFindSessionsCompleteDelegates.Remove(FindSessionHandle);
		ioSessionP->OnFindSessionsCompleteDelegates.RemoveAll(this);
	}
}

void UCusSoSubsystem::DoAfterFindSession(bool flag)
{
	//找到Session的话
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
	                                 FString::Printf(TEXT("Find Session Done %d"), NAME_GameSession));
	//
	//for (FOnlineSessionSearchResult result : search->SearchResults)
	for (int i = 0; i < search->SearchResults.Num(); i++)
	{
		//遍历SessionID和用户名
		FString id = search->SearchResults[i].GetSessionIdStr();
		FString user = search->SearchResults[i].Session.OwningUserName;
		//提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
		                                 FString::Printf(TEXT("Get A ID: %s, User: %s"), *id, *user));
		//获取网络变量
		FString testKey;
		search->SearchResults[i].Session.SessionSettings.Get(FName("TestKey"), testKey);
		if (testKey == FString("TestValue"))
		{
			//记录网络寻找结果
			ValidSession = &search->SearchResults[i];
			///
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
			                                 FString::Printf(TEXT("This Is A Valid Session")));
			break;
		}
		// }

		DELE_CusSoAfterFindSession.Broadcast(search->SearchResults, flag);
		//ioSessionP->OnFindSessionsCompleteDelegates.Remove(FindSessionHandle);
		ioSessionP->OnFindSessionsCompleteDelegates.RemoveAll(this);
	}
}

void UCusSoSubsystem::JoinGameSession()
{
	if (ioSessionP.IsValid() == false)
	{
		//网络接口为空的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Session Interface Is Nulllptr")));
		return;
	}

	if (ValidSession == nullptr)
	{
		//网络搜索结果为空的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("ValidSession Is Nulllptr")));
		return;
	}
	//获取本地玩家
	const ULocalPlayer* player = GetWorld()->GetFirstLocalPlayerFromController();
	//
	/*JoinSessionHandle = ioSessionP->OnJoinSessionCompleteDelegates.AddUObject(
		this, &UCusSoSubsystem::DoAfterJoinSession);*/
	ioSessionP->OnJoinSessionCompleteDelegates.AddUObject(
		this, &UCusSoSubsystem::DoAfterJoinSession);
	if (ioSessionP->JoinSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *ValidSession) == true)
	{
		//加入Session成功的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Try Join Session %d Success"), NAME_GameSession));
	}
	else
	{
		//假如Session失败的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Try Join Session %d Failed"), NAME_GameSession));
		DELE_CusSoAfterJoinSession.Broadcast(EOnJoinSessionCompleteResult::UnknownError);

		//ioSessionP->OnJoinSessionCompleteDelegates.Remove(JoinSessionHandle);

		ioSessionP->OnJoinSessionCompleteDelegates.RemoveAll(this);
	}
}


void UCusSoSubsystem::DoAfterJoinSession(FName name, EOnJoinSessionCompleteResult::Type flag)
{
	FString Address;
	if (ioSessionP->GetResolvedConnectString(NAME_GameSession, Address))
	{
		//加入成功后，解析地址并提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("GetResolvedConnectString Success: %s"), *Address));
		//加入世界
		CallClientTravel(Address);
	}
	else
	{
		//解析失败的提示
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("GetResolvedConnectString Failed")));
	}
	DELE_CusSoAfterJoinSession.Broadcast(flag);

	//ioSessionP->OnJoinSessionCompleteDelegates.Remove(JoinSessionHandle);
	ioSessionP->OnJoinSessionCompleteDelegates.RemoveAll(this);
}


void UCusSoSubsystem::DestroyGameSession()
{
}

void UCusSoSubsystem::DoAfterDestroySession(FName name, bool flag)
{
	DELE_CusSoAfterDestroySession.Broadcast(flag);
}
