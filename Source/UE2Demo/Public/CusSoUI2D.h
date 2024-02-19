// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CusSoUI2D.generated.h"

/**
 * 
 */
namespace EOnJoinSessionCompleteResult
{
	enum Type : int;
}

UCLASS()
class UE2DEMO_API UCusSoUI2D : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowAndFocus();
	void HideAndRemove();

	//Subsys指针
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCusSoSubsystem* cusSo;

protected:
	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	//注册进委托的函数
	UFUNCTION()
	void DFUNC_CusSoAfterCreateSession(bool flag);

	UFUNCTION()
	void DFUNC_CusSoAfterStartSession(bool flag);

	void DFUNC_CusSoAfterFindSession(const TArray<class FOnlineSessionSearchResult>& results, bool flag);

	void DFUNC_CusSoAfterJoinSession(EOnJoinSessionCompleteResult::Type result);

	UFUNCTION()
	void DFUNC_CusSoAfterDestroySession(bool flag);

private:
	//Button
	UPROPERTY(meta=(BindWidget))
	class UButton* BT_CreateSession;

	UPROPERTY(meta=(BindWidget))
	class UButton* BT_StartSession;

	UPROPERTY(meta=(BindWidget))
	class UButton* BT_FindSession;

	UPROPERTY(meta=(BindWidget))
	class UButton* BT_JoinSession;

	UPROPERTY(meta=(BindWidget))
	class UButton* BT_DestroySession;

	UPROPERTY(meta=(BindWidget))
	class UButton* BT_TravelToGameMap;

	//点击事件
	UFUNCTION()
	void OnClick_BT_CreateSession();
	UFUNCTION()
	void OnClick_BT_StartSession();
	UFUNCTION()
	void OnClick_BT_FindSession();
	UFUNCTION()
	void OnClick_BT_JoinSession();
	UFUNCTION()
	void OnClick_BT_DestroySession();
	UFUNCTION(BlueprintCallable)
	void OnClick_BT_TravelToGameMap(const FString& address);
};
