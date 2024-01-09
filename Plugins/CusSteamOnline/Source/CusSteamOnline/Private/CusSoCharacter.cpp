// Fill out your copyright notice in the Description page of Project Settings.


#include "CusSoCharacter.h"

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

void ACusSoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (OverlappingWeapon)
	// {
	// 	OverlappingWeapon->ShowWidget(true);
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
