// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CusSOCharacter.generated.h"

UCLASS()
class CUSSTEAMONLINE_API ACusSoCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	//按键
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void EquipKey();
	void CrouchKey();
	void AimKeyPress();
	void AimKeyRelease();

public:
	ACusSoCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//设置同步的属性
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//给武器调用，设置触碰到的武器
	void SetOverlappingWeapon(class AWeapon* weapon);

	virtual void PostInitializeComponents() override;

	bool GetIsArmed();
	bool GetIsAiming();

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UWidgetComponent* widgetComponent;

	//武器指针
	//UPROPERTY(Replicated)
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	//同步武器时调用的函数
	UFUNCTION()
	void OnRep_OverlappingWeapon(class AWeapon* LastWeapon);//此传参为赋值之前的值

	//武器系统组件
	UPROPERTY(VisibleAnywhere)
	class UCusSoWeaponComponent* weaponComp;
	
	//调用服务器方装备武器
	UFUNCTION(Server, Reliable)
	void CallServerEquipedAWeapon();

	//调用服务器方瞄准
	UFUNCTION(Server, Reliable)
	void CallServerBeAiming(bool flag);
};
