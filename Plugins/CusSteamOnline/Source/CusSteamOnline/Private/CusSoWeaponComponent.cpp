// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoWeaponComponent.h"

#include "CusSoCharacter.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCusSoWeaponComponent::UCusSoWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCusSoWeaponComponent::EquipAWeapon(AWeapon* weapon)
{
	if (equipper == nullptr || weapon == nullptr) return;

	equippedWeapon = weapon;
	equippedWeapon->SetWeaponState(EWeaponState::WS_Equpipped);
	const USkeletalMeshSocket* meshSocket = equipper->GetMesh()->GetSocketByName(FName("RightHandSockets"));
	if (meshSocket)
	{
		meshSocket->AttachActor(equippedWeapon, equipper->GetMesh());
	}
	equippedWeapon->SetOwner(equipper);
	//equippedWeapon->ShowWidget(false);

	equipper->GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts
void UCusSoWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UCusSoWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCusSoWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCusSoWeaponComponent, equippedWeapon);
	DOREPLIFETIME(UCusSoWeaponComponent, bIsAiming);
}
