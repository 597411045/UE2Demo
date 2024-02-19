// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

//武器状态
UENUM(BlueprintType)
enum class EWeaponState: uint8
{
	WS_Initial UMETA(DisplayName="Initial"),
	WS_Equpipped UMETA(DisplayName="Equipped"),
	WS_Dropped UMETA(DisplayName="Dropped"),
	WS_MAX UMETA(DisplayName="MAX"),
};

UCLASS()
class CUSSTEAMONLINE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	//武器的模型
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skelMesh;

	//武器的拾取范围
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* sphereColl;

	//武器状态
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_State)
	EWeaponState state;

	//当武器状态同步时调用
	UFUNCTION()
	void OnRep_State();

	//武器的UI提示
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* widgetComponent;

protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	//提供给角色调用开关提示
	void ShowWidget(bool flag);

	//提供给武器组件调用设置武器状态
	void SetWeaponState(EWeaponState enums);
};
