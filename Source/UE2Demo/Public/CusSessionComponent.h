// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CusSessionComponent.generated.h"


namespace EOnJoinSessionCompleteResult
{
	enum Type : int;
}

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE2DEMO_API UCusSessionComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:
	// Sets default values for this component's properties
	UCusSessionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	void OpenLobby();

	UFUNCTION(BlueprintCallable)
	void CallOpenLevel(const FString& address);

	UFUNCTION(BlueprintCallable)
	void CallClientTravel(const FString& address);

	//IOnlineSessionPtr
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> ioSessionP;

	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	//FOnCreateSessionComplete
	TDelegate<void(FName, bool)> afterCreateSession;
	void DoAfterCreateSession(FName name, bool flag);

	UFUNCTION(BlueprintCallable)
	void FindGameSession();

	TSharedPtr<class FOnlineSessionSearch> search;
	class FOnlineSessionSearchResult* ValidSession;

	//FOnFindSessionsComplete
	TDelegate<void(bool)> afterFindSession;
	void DoAfterFindSession(bool flag);

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	//FOnJoinSessionCompleteDelegate
	TDelegate<void(FName, EOnJoinSessionCompleteResult::Type)> afterJoinSession;
	void DoAfterJoinSession(FName name, EOnJoinSessionCompleteResult::Type flag);
};
