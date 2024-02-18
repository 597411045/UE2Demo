// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoHeadWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UCusSoHeadWidget::SetDisplayText(const FString& text)
{
	DisplayText->SetText(FText::FromString(text));
}

void UCusSoHeadWidget::ShowPlayerNetRole(APawn* pawn)
{
	//获取本地角色
	ENetRole netRole = pawn->GetLocalRole();
	//netRole = pawn->GetRemoteRole();
	FString str;
	switch (netRole)
	{
	//服务器所有同步的物体
	case ENetRole::ROLE_Authority:
		str = FString("ROLE_Authority");
		break;
	//客户端控制的物体
	case ENetRole::ROLE_AutonomousProxy:
		str = FString("ROLE_AutonomousProxy");
		break;
	//客户端其他不被控制的物体
	case ENetRole::ROLE_SimulatedProxy:
		str = FString("ROLE_SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		str = FString("ROLE_None");
		break;
	}
	SetDisplayText(str);
}

void UCusSoHeadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	//切换其他地图时，关闭提示
	RemoveFromParent();

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
