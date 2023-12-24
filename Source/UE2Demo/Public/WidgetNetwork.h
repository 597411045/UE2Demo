// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetNetwork.generated.h"

/**
 * 
 */
UCLASS()
class UE2DEMO_API UWidgetNetwork : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitClient(FString IP, int32 PORT);

	UFUNCTION(BlueprintCallable)
	void ClientConnect();

	UFUNCTION(BlueprintCallable)
	void ClientClose();

	UFUNCTION(BlueprintCallable)
	void ClientSend(const FString& message);


	UFUNCTION(BlueprintCallable)
	void InitServer(FString IP, int32 PORT);

	UFUNCTION(BlueprintCallable)
	void ServerListen();

	UFUNCTION(BlueprintCallable)
	void ServerBroadcast(const FString& message);

	UFUNCTION(BlueprintCallable)
	void ServerClose();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowInScreen(const FString& message,int slotIndex);


	class FTCPClient* client = nullptr;
	class FTCPServer* server = nullptr;
};
