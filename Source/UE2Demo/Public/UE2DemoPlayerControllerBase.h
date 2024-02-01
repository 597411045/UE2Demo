// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UE2DemoPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class UE2DEMO_API AUE2DemoPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
