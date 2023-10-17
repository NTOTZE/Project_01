// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem_Base.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "../00_Header/global.h"
#include "NiagaraComponent.h"

// Sets default values
ADropItem_Base::ADropItem_Base()
	: m_ItemID(EITEM_ID::NONE)
{
	//PrimaryActorTick.bCanEverTick = true;
	
	m_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_DropNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DropNiagara"));
	m_Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	SetRootComponent(m_Sphere);
	m_Sphere->SetSphereRadius(50.f);
	m_Sphere->SetCollisionProfileName(FName("InteractionActorProf"), true);

	m_DropNiagara->SetupAttachment(m_Sphere);

	m_Movement->InitialSpeed = 300.f;
	m_Movement->MaxSpeed = 300.f;
	m_Movement->bShouldBounce = true;
	m_Movement->Bounciness = 0.8f;
	m_Movement->Friction = 0.5f;
}

// Called when the game starts or when spawned
void ADropItem_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADropItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EITEM_ID ADropItem_Base::PickItem()
{
	SetLifeSpan(4.f);
	m_DropNiagara->Deactivate();
	m_Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	return m_ItemID;
}