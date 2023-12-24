// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FBaseSocket.h"

//DECLARE_DELEGATE_TwoParams(FOnServerResponseReceived, const int32&, bool);

class FTCPClient : public FBaseSocket
{
public:
	FTCPClient(UWidgetNetwork* widget);

	void RefInit(const FString& IP, const int32 PORT, const FString& description);
	void RefConnect();
	void RefSend(const FString& Message);
	bool RefClose();
	void RefStartReceiverThread();
	virtual ~FTCPClient();

	//Old
	// FOnServerResponseReceived& OnDataReceived()
	// {
	// 	return OnServerResponseReceived;
	// }

	//Old
	//class FUdpSocketReceiver* UDPReceiver;

	//Old
	//TArray<uint8> ReceivedData;

	//Old
	//int64 StartTime;

	//Old
	//FOnServerResponseReceived OnServerResponseReceived;
};
