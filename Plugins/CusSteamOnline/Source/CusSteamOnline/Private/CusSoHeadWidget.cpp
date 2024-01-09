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
	ENetRole netRole = pawn->GetLocalRole();
	//netRole = pawn->GetRemoteRole();
	FString str;
	switch (netRole)
	{
	case ENetRole::ROLE_Authority:
		str = FString("ROLE_Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		str = FString("ROLE_AutonomousProxy");
		break;
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
	RemoveFromParent();

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
