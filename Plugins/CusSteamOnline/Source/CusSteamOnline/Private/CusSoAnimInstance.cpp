// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoAnimInstance.h"

#include "CusSoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCusSoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	cha = Cast<ACusSoCharacter>(TryGetPawnOwner());
}

void UCusSoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (cha == nullptr)
	{
		cha = Cast<ACusSoCharacter>(TryGetPawnOwner());
	}
	if (cha == nullptr)
	{
		return;
	}

	FVector velocity = cha->GetVelocity();
	velocity.Z = 0;
	speed = velocity.Size();

	bIsInAir = cha->GetCharacterMovement()->IsFalling();
	bIsAccelerating = cha->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;
	bIsArmed = cha->GetIsArmed();
}
