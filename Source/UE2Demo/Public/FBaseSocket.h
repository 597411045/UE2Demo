#pragma once

#include "CoreMinimal.h"

class UWidgetNetwork;

class FBaseSocket : public FRunnable
{
public:
	FBaseSocket();
	virtual ~FBaseSocket();

	FSocket* Socket = nullptr;
	TSharedPtr<FInternetAddr> Addr;

	bool bAllowThreadRunning=false;
	FRunnableThread* fThread = nullptr;

	virtual bool Init() override;
	virtual void Stop() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	UWidgetNetwork* UserWidget = nullptr;
	int SlotIndex;
	TMap<FString, FBaseSocket*>* RefMap = nullptr;
	void (FBaseSocket::*fThreadAction)() = nullptr;

protected:
	bool InitSocket(const FString& IP, const int32 PORT, const FString& description);
	bool TryConnect();
	bool TryListen();
	void TryAccept();
	bool TrySendMessage(const FString& Message);
	void TryReceiveMessage();
	bool TryCloseAll();

	virtual void AddToMap(FBaseSocket* refer);
	void DebugOnUI(const FString& message);
	void ManualStartReceiverThread();

private:
	bool BuildAddr(const FString& IP, const int32 PORT);
	void BuildThread();

	FString StringFromBinaryArray(const TArray<uint8>& array);
};
