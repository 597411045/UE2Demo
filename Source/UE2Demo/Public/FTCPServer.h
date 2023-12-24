// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FBaseSocket.h"


class FTCPClient;

class FTCPServer : public FBaseSocket
{
public:
	FTCPServer(UWidgetNetwork* widget);

	virtual ~FTCPServer();
	void RefInit(const FString& IP, const int32 PORT, const FString& description);
	void RefListen();
	bool RefClose();
	void RefSendToAllClient(const FString& message);
	FTCPClient* GetSocketByString(FString key);

	TMap<FString, FTCPClient*> ClientMap;

	virtual void AddToMap(FBaseSocket* refer) override;
};
