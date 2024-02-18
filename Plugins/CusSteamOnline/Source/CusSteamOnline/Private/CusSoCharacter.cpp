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
	//摇臂跟随角色旋转
	springArm->bUsePawnControlRotation = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	//摄像机不跟随角色旋转
	camera->bUsePawnControlRotation = false;

	//角色不跟随控制器上下旋转
	bUseControllerRotationYaw = false;

	//角色自动转向前行的方向
	GetCharacterMovement()->bOrientRotationToMovement = true;

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	widgetComponent->SetupAttachment(RootComponent);

	weaponComp = CreateDefaultSubobject<UCusSoWeaponComponent>(TEXT("WeaponComponent"));
	//武器组件需要同步
	weaponComp->SetIsReplicated(true);

	//设置角色可以蹲下
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
		//获取左右旋转值
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		//获取前方单位向量
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, value);
	}
}

void ACusSoCharacter::MoveRight(float value)
{
	if (Controller != nullptr && value != 0)
	{
		//获取左右旋转值
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		//获取右方单位向量
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
	//如果是服务器方
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		weaponComp->EquipAWeapon(OverlappingWeapon);
	}
	else
	{
		//否则需要调用服务器方装备武器
		CallServerEquipedAWeapon();
	}
}

void ACusSoCharacter::CrouchKey()
{
	//装备武器后才可以蹲下
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
	//如果是服务器方
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		//直接执行
		weaponComp->bIsAiming = true;
		bUseControllerRotationYaw = true;
	}
	//如果是客户端方
	else
	{
		//调用服务器方要做的事情
		CallServerBeAiming(true);
		//如果是本地
		if (Controller && Controller->IsLocalController())
		{
			//自己也要做一次
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

	//设置武器指针需要同步，仅需同步拥有者
	//DOREPLIFETIME(ACusSoCharacter, OverlappingWeapon);
	DOREPLIFETIME_CONDITION(ACusSoCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ACusSoCharacter::SetOverlappingWeapon(AWeapon* weapon)
{
	//如果之前有武器，默认直接关闭拾取提示
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowWidget(false);
	}
	//赋值新武器
	OverlappingWeapon = weapon;
	//如果是本地
	if (Controller && Controller->IsLocalController())
	{
		//且有武器
		if (OverlappingWeapon)
		{
			//显示拾取提示
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
	//是否装备武器，要从武器组件获取
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
	//是否瞄准，要从武器组件获取
	return weaponComp->bIsAiming;
}

//同步武器时调用的函数
void ACusSoCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	//同步成功，并且有触碰武器
	if (OverlappingWeapon)
	{
		//显示触碰武器的拾取提示
		OverlappingWeapon->ShowWidget(true);
	}
	//同步成功，可能因为离开而没有触碰的武器
	else
	{
		//之前的武器关闭提示
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
