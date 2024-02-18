// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CusSoHeadWidget.generated.h"

//用于显示玩家角色信息

UCLASS()
class CUSSTEAMONLINE_API UCusSoHeadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* DisplayText;

	void SetDisplayText(const FString& text);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* pawn);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
