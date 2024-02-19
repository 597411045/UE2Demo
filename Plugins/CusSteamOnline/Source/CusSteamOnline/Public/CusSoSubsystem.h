// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Interfaces/OnlineSessionInterface.h"
#include "CusSoSubsystem.generated.h"

namespace EOnJoinSessionCompleteResult
{
	enum Type : int;
}

//一个内置参数类型的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCusSoAfterCreateSession, bool, bFlag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCusSoAfterStartSession, bool, bFlag);

//有自定义类型的2个参数的委托
DECLARE_MULTICAST_DELEGATE_TwoParams(FCusSoAfterFindSession, const TArray<FOnlineSessionSearchResult>& results,
                                     bool flag);

DECLARE_MULTICAST_DELEGATE_OneParam(FCusSoAfterJoinSession, EOnJoinSessionCompleteResult::Type result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCusSoAfterDestroySession, bool, bFlag);


UCLASS()
class CUSSTEAMONLINE_API UCusSoSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCusSoSubsystem();

	void CreateGameSession();

	void StartGameSession();

	void FindGameSession();

	void JoinGameSession();

	void DestroyGameSession();

	void BeginPlay();

	//提供给UI使用的委托
	FCusSoAfterCreateSession DELE_CusSoAfterCreateSession;
	FCusSoAfterStartSession DELE_CusSoAfterStartSession;
	FCusSoAfterFindSession DELE_CusSoAfterFindSession;
	FCusSoAfterJoinSession DELE_CusSoAfterJoinSession;
	FCusSoAfterDestroySession DELE_CusSoAfterDestroySession;

	UFUNCTION(BlueprintCallable)
	void CallServetTravel(bool bSeam);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString lobbyName = FString::Printf(TEXT("/Game/Work/Test/Lobby?listen"));

	UFUNCTION(BlueprintCallable)
	void CallClientTravel(const FString& address);

private:
	//IOnlineSessionPtr

	//网络会话
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> ioSessionP;
	//网络设置
	TSharedPtr<class FOnlineSessionSettings> setting;
	//网络搜索
	TSharedPtr<class FOnlineSessionSearch> search;

	//网络所谓结果
	class FOnlineSessionSearchResult* ValidSession;

	//注册给网络会话自动调用的委托，单一函数模式
	//class FDelegateHandle CreateSessionHandle;
	//class FDelegateHandle StartSessionHandle;
	//class FDelegateHandle FindSessionHandle;
	//class FDelegateHandle JoinSessionHandle;
	//class FDelegateHandle DestroySessionHandle;

	//委托，默认模式
	//FOnCreateSessionComplete
	//TDelegate<void(FName, bool)> afterCreateSession;
	//FOnStartSessionCompleteDelegate
	//TDelegate<void(FName, bool)> afterStartSession;
	//FOnFindSessionsComplete
	//TDelegate<void(bool)> afterFindSession;
	//FOnJoinSessionCompleteDelegate
	//TDelegate<void(FName, EOnJoinSessionCompleteResult::Type)> afterJoinSession;
	//FOnDestroySessionCompleteDelegate
	//TDelegate<void(FName, bool)> afterDestroySession;

	UFUNCTION(BlueprintCallable)
	void CallOpenLevel(const FString& address);

	void DoAfterCreateSession(FName name, bool flag);

	void DoAfterStartSession(FName name, bool flag);

	void DoAfterFindSession(bool flag);

	void DoAfterJoinSession(FName name, EOnJoinSessionCompleteResult::Type flag);

	void DoAfterDestroySession(FName name, bool flag);
};
