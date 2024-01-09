// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skelMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* sphereColl;

	UPROPERTY(VisibleAnywhere)
	EWeaponState state;

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
	void ShowWidget(bool flag);
};
