// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CusSoAnimInstance.generated.h"

/**
 *
 * 动画模板类
 */
UCLASS()
class CUSSTEAMONLINE_API UCusSoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private :
	//主角色指针
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class ACusSoCharacter* cha;

	//速度
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float speed;

	//是否在空中
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsInAir;

	//是否加速
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsAccelerating;

	//是否装备了武器
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsArmed;

	//是否蹲下
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsCrouched;

	//是否瞄准
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsAmining;

	//偏转度
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float YawOffset;
	
	//倾斜
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float Lean;
};
