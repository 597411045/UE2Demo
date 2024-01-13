// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoCharacter.h"

#include "CusSoWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon.h"

ACusSoCharacter::ACusSoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetMesh());
	springArm->TargetArmLength = 600;
	springArm->bUsePawnControlRotation = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	widgetComponent->SetupAttachment(RootComponent);

	weaponComp = CreateDefaultSubobject<UCusSoWeaponComponent>(TEXT("WeaponComponent"));
	weaponComp->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ACusSoCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACusSoCharacter::MoveForward(float value)
{
	if (Controller != nullptr && value != 0)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, value);
	}
}

void ACusSoCharacter::MoveRight(float value)
{
	if (Controller != nullptr && value != 0)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, value);
	}
}

void ACusSoCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void ACusSoCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ACusSoCharacter::EquipKey()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		weaponComp->EquipAWeapon(OverlappingWeapon);
	}
	else
	{
		CallServerEquipedAWeapon();
	}
}

void ACusSoCharacter::CrouchKey()
{
	if (GetIsArmed())
	{
		if (bIsCrouched)
		{
			UnCrouch();
		}
		else
		{
			Crouch();
		}
	}
}

void ACusSoCharacter::AimKeyPress()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		weaponComp->bIsAiming = true;
		bUseControllerRotationYaw = true;
	}
	else
	{
		CallServerBeAiming(true);
		if (Controller && Controller->IsLocalController())
		{
			weaponComp->bIsAiming = true;
			bUseControllerRotationYaw = true;
		}
	}
}

void ACusSoCharacter::AimKeyRelease()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		weaponComp->bIsAiming = false;
		bUseControllerRotationYaw = false;
	}
	else
	{
		CallServerBeAiming(false);
		if (Controller && Controller->IsLocalController())
		{
			weaponComp->bIsAiming = false;
			bUseControllerRotationYaw = false;
		}
	}
}

void ACusSoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (OverlappingWeapon)
	// {
	// 	OverlappingWeapon->ShowWidget(true);
	// }
	// if (!weaponComp->equippedWeapon)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s no weapon"), *GetName()));
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("%s have weapon"), *GetName()));
	// }
}

void ACusSoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACusSoCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACusSoCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACusSoCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACusSoCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &ACusSoCharacter::EquipKey);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ACusSoCharacter::CrouchKey);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ACusSoCharacter::AimKeyPress);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ACusSoCharacter::AimKeyRelease);
}

void ACusSoCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ACusSoCharacter, OverlappingWeapon);
	DOREPLIFETIME_CONDITION(ACusSoCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ACusSoCharacter::SetOverlappingWeapon(AWeapon* weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowWidget(false);
	}
	OverlappingWeapon = weapon;
	if (Controller && Controller->IsLocalController())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowWidget(true);
		}
	}
}

void ACusSoCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	weaponComp->equipper = this;
}

bool ACusSoCharacter::GetIsArmed()
{
	if (weaponComp->equippedWeapon)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ACusSoCharacter::GetIsAiming()
{
	return weaponComp->bIsAiming;
}

void ACusSoCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowWidget(true);
	}
	else
	{
		LastWeapon->ShowWidget(false);
	}
	// if (LastWeapon)
	// {
	// 	LastWeapon->ShowWidget(false);
	// }
}

void ACusSoCharacter::CallServerBeAiming_Implementation(bool flag)
{
	weaponComp->bIsAiming = flag;
	bUseControllerRotationYaw = flag;
}

void ACusSoCharacter::CallServerEquipedAWeapon_Implementation()
{
	weaponComp->EquipAWeapon(OverlappingWeapon);
}
