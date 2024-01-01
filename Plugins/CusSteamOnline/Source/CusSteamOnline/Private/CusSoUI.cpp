// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoUI.h"
#include "CusSoSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"


void UCusSoUI::ShowAndFocus()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		FInputModeUIOnly mui;
		mui.SetWidgetToFocus(TakeWidget());
		mui.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		pc->SetInputMode(mui);
		pc->SetShowMouseCursor(true);
	}

	UGameInstance* gi = GetGameInstance();
	if (gi)
	{
		cusSo = gi->GetSubsystem<UCusSoSubsystem>();
		cusSo->DELE_CusSoAfterCreateSession.AddDynamic(this, &UCusSoUI::DFUNC_CusSoAfterCreateSession);
		cusSo->DELE_CusSoAfterStartSession.AddDynamic(this, &UCusSoUI::DFUNC_CusSoAfterCreateSession);
		cusSo->DELE_CusSoAfterFindSession.AddUObject(this, &UCusSoUI::DFUNC_CusSoAfterFindSession);
		cusSo->DELE_CusSoAfterJoinSession.AddUObject(this, &UCusSoUI::DFUNC_CusSoAfterJoinSession);
		cusSo->DELE_CusSoAfterDestroySession.AddDynamic(this, &UCusSoUI::DFUNC_CusSoAfterCreateSession);
	}
}

void UCusSoUI::HideAndRemove()
{
	RemoveFromParent();
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		FInputModeGameOnly mgame;
		pc->SetInputMode(mgame);
		pc->SetShowMouseCursor(false);
	}
}

bool UCusSoUI::Initialize()
{
	if (Super::Initialize() == false)
	{
		return false;
	}

	BT_CreateSession->OnClicked.AddDynamic(this, &UCusSoUI::OnClick_BT_CreateSession);

	BT_StartSession->OnClicked.AddDynamic(this, &UCusSoUI::OnClick_BT_StartSession);

	BT_FindSession->OnClicked.AddDynamic(this, &UCusSoUI::OnClick_BT_FindSession);

	BT_JoinSession->OnClicked.AddDynamic(this, &UCusSoUI::OnClick_BT_JoinSession);

	BT_DestroySession->OnClicked.AddDynamic(this, &UCusSoUI::OnClick_BT_DestroySession);

	return true;
}

void UCusSoUI::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	HideAndRemove();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UCusSoUI::DFUNC_CusSoAfterCreateSession(bool flag)
{
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

void UCusSoUI::DFUNC_CusSoAfterStartSession(bool flag)
{
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

void UCusSoUI::DFUNC_CusSoAfterFindSession(const TArray<FOnlineSessionSearchResult>& results, bool flag)
{
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

void UCusSoUI::DFUNC_CusSoAfterJoinSession(EOnJoinSessionCompleteResult::Type result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
	                                 FString::Printf(TEXT("EOnJoinSessionCompleteResult")));
}

void UCusSoUI::DFUNC_CusSoAfterDestroySession(bool flag)
{
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

void UCusSoUI::OnClick_BT_CreateSession()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_CreateSession")));
	if (cusSo)
	{
		cusSo->CreateGameSession();
	}
}

void UCusSoUI::OnClick_BT_StartSession()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_StartSession")));
	if (cusSo)
	{
		cusSo->StartGameSession();
	}
}

void UCusSoUI::OnClick_BT_FindSession()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_FindSession")));
	if (cusSo)
	{
		cusSo->FindGameSession();
	}
}

void UCusSoUI::OnClick_BT_JoinSession()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_JoinSession")));
	if (cusSo)
	{
		cusSo->JoinGameSession();
	}
}

void UCusSoUI::OnClick_BT_DestroySession()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("OnClick_BT_DestroySession")));
	if (cusSo)
	{
		cusSo->DestroyGameSession();
	}
}
