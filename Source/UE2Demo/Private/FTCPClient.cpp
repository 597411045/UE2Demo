// Fill out your copyright notice in the Description page of Project Settings.


#include "FTCPClient.h"


FTCPClient::FTCPClient(UWidgetNetwork* widget)
{
	UserWidget = widget;
	SlotIndex = 0;
}

void FTCPClient::RefInit(const FString& IP, const int32 PORT, const FString& description)
{
	InitSocket(IP, PORT, description);
}

void FTCPClient::RefConnect()
{
	TryConnect();
}

void FTCPClient::RefSend(const FString& Message)
{
	TrySendMessage(Message);
}

bool FTCPClient::RefClose()
{
	return TryCloseAll();
}

void FTCPClient::RefStartReceiverThread()
{
	ManualStartReceiverThread();
}

FTCPClient::~FTCPClient()
{
	TryCloseAll();
}
