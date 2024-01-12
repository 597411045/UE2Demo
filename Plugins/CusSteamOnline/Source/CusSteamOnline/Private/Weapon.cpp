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


	skelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMesh"));
	SetRootComponent(skelMesh);


	skelMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	skelMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
	skelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	sphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColl"));
	sphereColl->SetupAttachment(RootComponent);
	sphereColl->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereColl->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	DOREPLIFETIME(AWeapon, state);
}

void AWeapon::OnRep_State()
{
	switch (state)
	{
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
	case EWeaponState::WS_Equpipped:
		ShowWidget(false);
		sphereColl->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}
