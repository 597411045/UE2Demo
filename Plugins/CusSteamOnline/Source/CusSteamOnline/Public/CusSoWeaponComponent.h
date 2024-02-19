// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CusSoWeaponComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUSSTEAMONLINE_API UCusSoWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCusSoWeaponComponent();
	friend class ACusSoCharacter;

	//提供外部调用，装备武器
	void EquipAWeapon(class AWeapon* weapon);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	//已装备的武器
	UPROPERTY(Replicated)
	class AWeapon* equippedWeapon;

	//装备者
	class ACusSoCharacter* equipper;

	//是否在瞄准
	UPROPERTY(Replicated)
	bool bIsAiming;
};
