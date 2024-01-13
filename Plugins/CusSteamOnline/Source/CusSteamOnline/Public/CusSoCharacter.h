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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	//UPROPERTY(Replicated)
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(class AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere)
	class UCusSoWeaponComponent* weaponComp;

	UFUNCTION(Server, Reliable)
	void CallServerEquipedAWeapon();

	UFUNCTION(Server, Reliable)
	void CallServerBeAiming(bool flag);
};
