#include "FBaseSocket.h"

#include <sstream>


#include "WidgetNetwork.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include <WinSock2.h>
#include <wincrypt.h>
#include <Winldap.h>

#define CURL_STATICLIB 
#include "../../ThirdParty/include/curl/curl.h"


FBaseSocket::FBaseSocket()
{
}

FBaseSocket::~FBaseSocket()
{
	TryCloseAll();
}

bool FBaseSocket::InitSocket(const FString& IP, const int32 PORT, const FString& description)
{
	if (Socket == nullptr)
	{
		if (BuildAddr(IP, PORT))
		{
			Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, description, false);
			DebugOnUI(FString::Printf(TEXT("InitSocket Success")));
			return true;
		}
	}
	DebugOnUI(FString::Printf(TEXT("InitSocket Failed, Socket Is Not Null")));
	return false;
}

bool FBaseSocket::TryConnect()
{
	AsyncTask(ENamedThreads::AnyThread, [=]()
	{
		if (Socket && Addr)
		{
			if (Socket->GetConnectionState() == ESocketConnectionState::SCS_NotConnected)
			{
				if (Socket->Connect(*Addr))
				{
					DebugOnUI(FString::Printf(TEXT("Connect Success:%s"), *Addr->ToString(true)));
					ManualStartReceiverThread();
				}
				else
				{
					DebugOnUI(FString::Printf(TEXT("Connect Failed:%s"), *Addr->ToString(true)));
				}
			}
			else
			{
				DebugOnUI(FString::Printf(TEXT("Socket State Not Clear")));
			}
		}
		else
		{
			DebugOnUI(FString::Printf(TEXT("Socket Or Addr Not Valid")));
		}
	});

	return false;
}

bool FBaseSocket::TryListen()
{
	if (Socket && Addr)
	{
		//Old
		// Socket = FTcpSocketBuilder(TEXT("Socket Listener"))
		// 				   .AsReusable()
		// 				   .AsBlocking()
		// 				   .BoundToAddress(ServerAddr)
		// 				   .BoundToPort(PORT)
		// 				   .Listening(8)
		// 				   .WithReceiveBufferSize(1024)
		// 				   .WithSendBufferSize(1024);
		if (Socket->Bind(*Addr))
		{
			if (Socket->Listen(5))
			{
				DebugOnUI(FString::Printf(TEXT("Listen Success:%s"), *Addr->ToString(true)));

				fThreadAction = &FBaseSocket::TryAccept;
				BuildThread();
				return true;
			}
			else
			{
				DebugOnUI(FString::Printf(TEXT("Listen Failed:%s"), *Addr->ToString(true)));
			}
		}
		else
		{
			DebugOnUI(FString::Printf(TEXT("Bind Failed:%s"), *Addr->ToString(true)));
		}
	}
	else
	{
		DebugOnUI(FString::Printf(TEXT("Socket Or Addr Not Valid")));
	}
	return false;
}

void FBaseSocket::TryAccept()
{
	if (Socket)
	{
		bool flag;
		while (Socket->HasPendingConnection(flag) && flag)
		{
			TSharedPtr<FInternetAddr> tmpAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
			FSocket* tmpSocket = Socket->Accept(*tmpAddr,TEXT("Accepted Client Conn"));
			FString time = FDateTime::Now().ToFormattedString(TEXT("%Y-%m-%d %H:%M:%S"));
			DebugOnUI(FString::Printf(TEXT("Accept A Client:%s %s"), *time, *tmpAddr->ToString(true),
			                          tmpAddr->GetPort()));


			FBaseSocket* tmpClient = new FBaseSocket();
			tmpClient->Socket = tmpSocket;
			tmpClient->Addr = tmpAddr;
			tmpClient->UserWidget = UserWidget;
			tmpClient->SlotIndex = SlotIndex;
			AddToMap(tmpClient);
		}
	}
	else
	{
		DebugOnUI(FString::Printf(TEXT("Socket Null Or Not Connected")));
	}
}

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
	std::string data((char*)buffer, size * nmemb);
	*((std::stringstream*)userp) << data << std::endl;
	return size * nmemb;
}

bool FBaseSocket::TrySendMessage(const FString& Message)
{
	if (Socket && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	{
		FString http =
			TEXT("GET / HTTP/1.0\r\nHost: www.baidu.com\r\nAccept: */*\r\nAuthorization: Basic Og==\r\n\r\n");
		TArray<uint8> ToSendData;
		ToSendData.Init(0, 0);
		for (auto chars : http.GetCharArray())
		{
			ToSendData.Push(chars);
		}
	
		//const TCHAR* MessageData = *Message;
		FString time = FDateTime::Now().ToFormattedString(TEXT("%Y-%m-%d %H:%M:%S"));
		//Socket->Send((uint8*)TCHAR_TO_UTF8(MessageData), FCString::Strlen(MessageData), BytesSent);
	
		int32 BytesSent = 0;
	
		if (Socket->Send(ToSendData.GetData(), ToSendData.Num(), BytesSent))
		{
			DebugOnUI(FString::Printf(TEXT("%s sent:%s"), *time, *Message));
		}
		else
		{
			DebugOnUI(FString::Printf(TEXT("Send Failed, Try Reconnect")));
		}
		return true;
	}
	else
	{
		DebugOnUI(FString::Printf(TEXT("Socket Null Or Not Connected")));
	}

	// CURL* curl = curl_easy_init();
	//
	// std::stringstream out;
	//
	// if (curl)
	// {
	// 	CURLcode res;
	// 	curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
	// 	curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:7890");
	//
	// 	curl_slist* headers = nullptr;
	// 	headers = curl_slist_append(headers, "Content-Type: application/json");
	// 	headers = curl_slist_append(
	// 		headers, "Authorization: Bearer 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");
	// 	headers = curl_slist_append(headers, "OpenAI-Organization: org-8NFrG4csr9SaipTNY8iOCOFx");
	// 	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	//
	// 	const char* json =
	// 		"{\"model\": \"gpt-3.5-turbo\",\"messages\" : [{\"role\": \"user\", \"content\" : \"Say this is a test!\"}] ,\"temperature\" : 0.7}";
	// 	curl_easy_setopt(curl, CURLOPT_POST, true);
	// 	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
	//
	// 	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
	// 	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	//
	// 	res = curl_easy_perform(curl);
	//
	// 	curl_slist_free_all(headers);
	//
	// 	std::string a = out.str();
	// 	MessageBoxA(0, a.c_str(), 0, NULL);
	// }


	return false;
}

void FBaseSocket::TryReceiveMessage()
{
	if (Socket && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	{
		//uint8 Data[1024];
		TArray<uint8> ReceivedData;

		uint32 Size;
		int32 BytesReceived = 0;
		while (Socket->HasPendingData(Size))
		{
			ReceivedData.Init(0, FMath::Min(Size, 65507u));
			//if (Socket->Recv(Data, sizeof(Data), BytesReceived, ESocketReceiveFlags::None))
			if (Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesReceived))
			{
				if (BytesReceived > 0)
				{
					//FString Message = FString(UTF8_TO_TCHAR((const char*)Data));
					FString Message = StringFromBinaryArray(ReceivedData);

					FString time = FDateTime::Now().ToFormattedString(TEXT("%Y-%m-%d %H:%M:%S"));
					DebugOnUI(FString::Printf(TEXT("%s recv:%s"), *time, *Message));
					//auto r = OnServerResponseReceived.ExecuteIfBound(EndTime - StartTime, true);
					//bAllowThreadRunning = false;

					// TArray<FString> Messages;
					// ReceivedString.ParseIntoArray(Messages,TEXT("\n"), true);
					// for (const FString& Message : Messages)
					// {
					// }
				}
			}
		}
	}
	else
	{
		DebugOnUI(FString::Printf(TEXT("Socket Null Or Not Connected")));
	}
}

#pragma push_macro("SetPort")
#undef SetPort
bool FBaseSocket::BuildAddr(const FString& IP, const int32 PORT)
{
	Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bValid;

	FResolveInfo* info = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetHostByName(TCHAR_TO_ANSI(ToCStr(IP)));

	while (!info->IsComplete());
	if (info->GetErrorCode() == 0)
	{
		bValid = true;
		const FInternetAddr* tmpAddr = &info->GetResolvedAddress();
		uint32 ip = 0;
		tmpAddr->GetIp(ip);
		Addr->SetIp(ip);
		Addr->SetPort(PORT);
		DebugOnUI(FString::Printf(TEXT("Address To :%s"), *tmpAddr->ToString(true)));
	}
	else
	{
		Addr->SetIp(*IP, bValid);
		if (bValid)
		{
			Addr->SetPort(PORT);
			DebugOnUI(FString::Printf(TEXT("Direct IP Valid:%s"), *IP));
		}
		else
		{
			DebugOnUI(FString::Printf(TEXT("Direct IP Not Valid:%s"), *IP));
			TryCloseAll();
		}
	}

	return bValid;
}
#pragma pop_macro("SetPort")

void FBaseSocket::BuildThread()
{
	bAllowThreadRunning = true;
	fThread = FRunnableThread::Create(this, *Socket->GetDescription());
	DebugOnUI(FString::Printf(TEXT("Thread Ready")));
}

bool FBaseSocket::Init()
{
	if (Socket && Addr)
	{
		DebugOnUI(FString::Printf(TEXT("Thead Init Success")));
		return true;
	}
	else
	{
		DebugOnUI(FString::Printf(TEXT("Thead Init Failed")));
		return false;
	}
}

void FBaseSocket::Stop()
{
	if (fThread)
	{
		DebugOnUI(FString::Printf(TEXT("Thead Close")));
	}
}

uint32 FBaseSocket::Run()
{
	while (bAllowThreadRunning)
	{
		(this->*fThreadAction)();

		FPlatformProcess::Sleep(1.f);
	}

	return 0;
}

void FBaseSocket::Exit()
{
	FRunnable::Exit();
}

void FBaseSocket::DebugOnUI(const FString& message)
{
	if (UserWidget)
	{
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UserWidget->ShowInScreen(message, SlotIndex);
		});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UserWidget is null"));
	}
}

void FBaseSocket::ManualStartReceiverThread()
{
	fThreadAction = &FBaseSocket::TryReceiveMessage;
	BuildThread();
}

FString FBaseSocket::StringFromBinaryArray(const TArray<uint8>& array)
{
	FString tmp;
	for (auto a : array)
	{
		tmp += TCHAR(a);
	}
	return tmp;
	//FString tmp = BytesToString(array.GetData(), array.Num());
	//return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(array.GetData())));
}

bool FBaseSocket::TryCloseAll()
{
	if (bAllowThreadRunning)
	{
		bAllowThreadRunning = false;
	}
	if (fThreadAction)
	{
		fThread->Kill(false);
	}
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		//delete Socket;
		Socket = nullptr;
		DebugOnUI(FString::Printf(TEXT("Socket Close")));
	}
	return true;
}

void FBaseSocket::AddToMap(FBaseSocket* refer)
{
}
