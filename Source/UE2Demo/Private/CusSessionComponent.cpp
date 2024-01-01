// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSessionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

// Sets default values for this component's properties
UCusSessionComponent::UCusSessionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCusSessionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//
	afterCreateSession = TDelegate<void(FName, bool)>::CreateUObject(this, &UCusSessionComponent::DoAfterCreateSession);
	afterFindSession = TDelegate<void(bool)>::CreateUObject(this, &UCusSessionComponent::DoAfterFindSession);
	afterJoinSession = TDelegate<void(FName name, EOnJoinSessionCompleteResult::Type flag)>::CreateUObject(
		this, &UCusSessionComponent::DoAfterJoinSession);
	//
	IOnlineSubsystem* ioSub = IOnlineSubsystem::Get();
	if (ioSub)
	{
		ioSessionP = ioSub->GetSessionInterface();
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(
			                                 TEXT("Get A Session Interface %s"),
			                                 *ioSub->GetSubsystemName().ToString()));
	}
}


// Called every frame
void UCusSessionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCusSessionComponent::OpenLobby()
{
	UWorld* world = GetWorld();
	if (world)
	{
		world->ServerTravel("/Game/Work/Test/Lobby?listen");
	}
}

void UCusSessionComponent::CallOpenLevel(const FString& address)
{
	UGameplayStatics::OpenLevel(this, *address);
}

void UCusSessionComponent::CallClientTravel(const FString& address)
{
	APlayerController* PC = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}

void UCusSessionComponent::CreateGameSession()
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
		                                 FString::Printf(TEXT("Destroyed A Old Session %d"), NAME_GameSession));
	}
	//
	TSharedPtr<FOnlineSessionSettings> setting = MakeShareable(new FOnlineSessionSettings());
	setting->bIsLANMatch = false;
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
	ioSessionP->AddOnCreateSessionCompleteDelegate_Handle(afterCreateSession);
	ioSessionP->CreateSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *setting);
	///
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
	                                 FString::Printf(TEXT("Createing A Session %d"), NAME_GameSession));
}

void UCusSessionComponent::DoAfterCreateSession(FName name, bool flag)
{
	if (flag)
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green,
		                                 FString::Printf(TEXT("Create A Session %s Success"), *name.ToString()));
		//
		OpenLobby();
	}
	else
	{
		///
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(TEXT("Create A Session %s Failed"), *name.ToString()));
	}
}

void UCusSessionComponent::FindGameSession()
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
	ioSessionP->AddOnFindSessionsCompleteDelegate_Handle(afterFindSession);
	ioSessionP->FindSessions(*player->GetPreferredUniqueNetId(), search.ToSharedRef());
	///
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow,
	                                 FString::Printf(TEXT("Finding A Session %d"), NAME_GameSession));
}

void UCusSessionComponent::DoAfterFindSession(bool flag)
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
}

void UCusSessionComponent::JoinGameSession()
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
	ioSessionP->AddOnJoinSessionCompleteDelegate_Handle(afterJoinSession);
	ioSessionP->JoinSession(*player->GetPreferredUniqueNetId(), NAME_GameSession, *ValidSession);
}

void UCusSessionComponent::DoAfterJoinSession(FName name, EOnJoinSessionCompleteResult::Type flag)
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
}
