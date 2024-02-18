// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoAnimInstance.h"

#include "CusSoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCusSoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//给主角色指针赋值
	cha = Cast<ACusSoCharacter>(TryGetPawnOwner());
}

void UCusSoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (cha == nullptr)
	{
		//给主角色指针赋值
		cha = Cast<ACusSoCharacter>(TryGetPawnOwner());
	}
	if (cha == nullptr)
	{
		return;
	}

	//获取速度
	FVector velocity = cha->GetVelocity();
	velocity.Z = 0;
	speed = velocity.Size();

	//获取是否在空中
	bIsInAir = cha->GetCharacterMovement()->IsFalling();
	//获取是否加速
	bIsAccelerating = cha->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;
	//获取是否装备武器
	bIsArmed = cha->GetIsArmed();
	//获取是否蹲下
	bIsCrouched = cha->bIsCrouched;
	//获取是否标准
	bIsAmining = cha->GetIsAiming();

	//TODO 偏转倾斜
	FRotator AimRotation = cha->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(cha->GetVelocity());
}
