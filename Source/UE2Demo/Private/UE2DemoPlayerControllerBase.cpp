// Fill out your copyright notice in the Description page of Project Settings.


#include "UE2DemoPlayerControllerBase.h"

#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"

void AUE2DemoPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	TSharedRef<FNavigationConfig> navi = MakeShared<FNavigationConfig>();
	navi->bKeyNavigation = true;
	navi->bTabNavigation = false;
	navi->bAnalogNavigation = true;
	FSlateApplication::Get().SetNavigationConfig(navi);
}
