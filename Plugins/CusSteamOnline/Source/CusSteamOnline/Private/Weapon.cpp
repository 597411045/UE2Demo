// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "CusSoCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	//设置Mesh
	skelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMesh"));
	SetRootComponent(skelMesh);

	//设置Mesh碰撞
	skelMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	skelMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
	skelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//设置范围碰撞
	sphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColl"));
	sphereColl->SetupAttachment(RootComponent);
	sphereColl->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereColl->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//设置UI
	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WSidgetComponent"));
	widgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	widgetComponent->SetVisibility(false);
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		//服务器端允许碰撞
		sphereColl->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		sphereColl->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
		sphereColl->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		sphereColl->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
}

// Called every frameS
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//实时同步的变量
	DOREPLIFETIME(AWeapon, state);
}

void AWeapon::OnRep_State()
{
	switch (state)
	{
	//同步为已装备的时候，关闭提示
	case EWeaponState::WS_Equpipped:
		ShowWidget(false);
		break;
	default: ;
	}
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	//触碰到时，设置触碰者触碰到的武器
	ACusSoCharacter* cha = Cast<ACusSoCharacter>(OtherActor);
	if (cha)
	{
		//widgetComponent->SetVisibility(true);
		cha->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//离开触碰到时，移除触碰者触碰到的武器
	ACusSoCharacter* cha = Cast<ACusSoCharacter>(OtherActor);
	if (cha)
	{
		cha->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::ShowWidget(bool flag)
{
	widgetComponent->SetVisibility(flag);
}

void AWeapon::SetWeaponState(EWeaponState enums)
{
	state = enums;
	switch (state)
	{
	//设置武器状态为装备的时候，关闭提示，取消碰撞
	case EWeaponState::WS_Equpipped:
		ShowWidget(false);
		sphereColl->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}
