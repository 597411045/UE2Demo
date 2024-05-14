// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoUI2D.h"
#include "CusSoSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"


void UCusSoUI2D::ShowAndFocus()
{
	//显示UI
	//AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	//设置输入模式和鼠标显示
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		//FInputModeUIOnly mui;
		FInputModeGameAndUI mui;
		mui.SetWidgetToFocus(TakeWidget());
		mui.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		pc->SetInputMode(mui);
		pc->SetShowMouseCursor(true);
	}

	UGameInstance* gi = GetGameInstance();
	if (gi)
	{
		//获取Subsys
		cusSo = gi->GetSubsystem<UCusSoSubsystem>();
		//注册委托
		cusSo->DELE_CusSoAfterCreateSession.AddDynamic(this, &UCusSoUI2D::DFUNC_CusSoAfterCreateSession);
		cusSo->DELE_CusSoAfterStartSession.AddDynamic(this, &UCusSoUI2D::DFUNC_CusSoAfterCreateSession);
		cusSo->DELE_CusSoAfterFindSession.AddUObject(this, &UCusSoUI2D::DFUNC_CusSoAfterFindSession);
		cusSo->DELE_CusSoAfterJoinSession.AddUObject(this, &UCusSoUI2D::DFUNC_CusSoAfterJoinSession);
		cusSo->DELE_CusSoAfterDestroySession.AddDynamic(this, &UCusSoUI2D::DFUNC_CusSoAfterCreateSession);
		//->lobbyName = FString::Printf(TEXT("/Game/Work/Demo2/Demo2_1?listen"));
	}
}

void UCusSoUI2D::HideAndRemove()
{
	//隐藏UI
	RemoveFromParent();
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		FInputModeGameOnly mgame;
		pc->SetInputMode(mgame);
		pc->SetShowMouseCursor(false);
	}
}

bool UCusSoUI2D::Initialize()
{
	if (Super::Initialize() == false)
	{
		return false;
	}


	//注册点击事件
	BT_CreateSession->OnClicked.AddDynamic(this, &UCusSoUI2D::OnClick_BT_CreateSession);

	BT_StartSession->OnClicked.AddDynamic(this, &UCusSoUI2D::OnClick_BT_StartSession);

	BT_FindSession->OnClicked.AddDynamic(this, &UCusSoUI2D::OnClick_BT_FindSession);

	BT_JoinSession->OnClicked.AddDynamic(this, &UCusSoUI2D::OnClick_BT_JoinSession);

	BT_DestroySession->OnClicked.AddDynamic(this, &UCusSoUI2D::OnClick_BT_DestroySession);

	return true;
}

void UCusSoUI2D::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	//切换到新世界时，自动隐藏
	HideAndRemove();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UCusSoUI2D::DFUNC_CusSoAfterCreateSession(bool flag)
{
	//仅作提示
	if (flag)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterCreateSession Success")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterCreateSession Failed")));
	}
}

void UCusSoUI2D::DFUNC_CusSoAfterStartSession(bool flag)
{
	//仅作提示
	if (flag)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterStartSession Success")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterStartSession Failed")));
	}
}

void UCusSoUI2D::DFUNC_CusSoAfterFindSession(const TArray<FOnlineSessionSearchResult>& results, bool flag)
{
	//仅作提示
	if (flag)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterFindSession Success")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterFindSession Failed")));
	}
}

void UCusSoUI2D::DFUNC_CusSoAfterJoinSession(EOnJoinSessionCompleteResult::Type result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
	                                 FString::Printf(TEXT("EOnJoinSessionCompleteResult")));
}

void UCusSoUI2D::DFUNC_CusSoAfterDestroySession(bool flag)
{
	//仅作提示
	if (flag)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterDestroySession Success")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("DFUNC_CusSoAfterDestroySession Failed")));
	}
}

void UCusSoUI2D::OnClick_BT_CreateSession()
{
	//点击后，创建Session
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_CreateSession")));
	if (cusSo)
	{
		cusSo->CreateGameSession();
	}
}

void UCusSoUI2D::OnClick_BT_StartSession()
{
	//暂未使用
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_StartSession")));
	if (cusSo)
	{
		cusSo->StartGameSession();
	}
}

void UCusSoUI2D::OnClick_BT_FindSession()
{
	//点击后，寻找Session
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_FindSession")));
	if (cusSo)
	{
		cusSo->FindGameSession();
	}
}

void UCusSoUI2D::OnClick_BT_JoinSession()
{
	//点击后，加入Session
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_JoinSession")));
	if (cusSo)
	{
		cusSo->JoinGameSession();
	}
}

void UCusSoUI2D::OnClick_BT_DestroySession()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_DestroySession")));
	if (cusSo)
	{
		cusSo->DestroyGameSession();
	}
}

void UCusSoUI2D::OnClick_BT_TravelToGameMap(const FString& address)
{
	//点击后，服务端切换到指定世界
	cusSo->CallServetTravel(address,false);
}
