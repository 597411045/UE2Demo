// Fill out your copyright notice in the Description page of Project Settings.


#include "FTCPServer.h"
#include "FTCPClient.h"
#include "WidgetNetwork.h"

FTCPServer::FTCPServer(UWidgetNetwork* widget)
{
	UserWidget = widget;
	SlotIndex = 1;
}

FTCPServer::~FTCPServer()
{
	TryCloseAll();
}

void FTCPServer::RefInit(const FString& IP, const int32 PORT, const FString& description)
{
	InitSocket(IP, PORT, description);
}

void FTCPServer::RefListen()
{
	TryListen();
}

bool FTCPServer::RefClose()
{
	for (auto client : ClientMap)
	{
		client.Value->RefClose();
	}
	return TryCloseAll();
}

void FTCPServer::RefSendToAllClient(const FString& message)
{
	for (auto client : ClientMap)
	{
		client.Value->RefSend(message);
	}
}

FTCPClient* FTCPServer::GetSocketByString(FString key)
{
	return *ClientMap.Find(key);
}

void FTCPServer::AddToMap(FBaseSocket* refer)
{
	FTCPClient* tmpClient = (FTCPClient*)refer;
	ClientMap.Add(FGuid::NewGuid().ToString(), tmpClient);
	DebugOnUI("A Client Added");
	tmpClient->RefStartReceiverThread();
}
