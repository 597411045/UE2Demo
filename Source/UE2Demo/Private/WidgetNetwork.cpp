// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetNetwork.h"
#include "FTCPClient.h"
#include "FTCPServer.h"


void UWidgetNetwork::InitClient(FString IP, int32 PORT)
{
	if (client == nullptr)
	{
		client = new FTCPClient(this);
		client->RefInit(IP, PORT, "Client");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Client Not Null");
	}
}

void UWidgetNetwork::ClientConnect()
{
	if (client)
	{
		client->RefConnect();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Client Null");
	}
}

void UWidgetNetwork::ClientClose()
{
	if (client)
	{
		if (client->RefClose())
		{
			client = nullptr;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Client Null");
	}
}

void UWidgetNetwork::ClientSend(const FString& message)
{
	if (client)
	{
		client->RefSend(message);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Client Null");
	}
}

void UWidgetNetwork::InitServer(FString IP, int32 PORT)
{
	if (server == nullptr)
	{
		server = new FTCPServer(this);
		server->RefInit(IP, PORT, "Server");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Server Not Null");
	}
}

void UWidgetNetwork::ServerListen()
{
	if (server)
	{
		server->RefListen();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Server Null");
	}
}

void UWidgetNetwork::ServerBroadcast(const FString& message)
{
	if (server)
	{
		server->RefSendToAllClient(message);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Server Null");
	}
}

void UWidgetNetwork::ServerClose()
{
	if (server)
	{
		if (server->RefClose())
		{
			server = nullptr;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Server Null");
	}
}
