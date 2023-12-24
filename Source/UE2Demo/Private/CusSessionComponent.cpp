// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSessionComponent.h"
#include "Kismet/GameplayStatics.h"

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
		world->ServerTravel("Game/Work/Test/Lobby?listen");
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
