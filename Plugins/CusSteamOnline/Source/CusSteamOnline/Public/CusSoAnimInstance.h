// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CusSoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CUSSTEAMONLINE_API UCusSoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private :
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class ACusSoCharacter* cha;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float speed;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsArmed;
};
