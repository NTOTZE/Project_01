// Fill out your copyright notice in the Description page of Project Settings.


#include "BossDoor_Base.h"

#include "../02_GameMode/GameMode_Main_Base.h"
#include "../03_Character/Character_Base.h"
#include "../05_Monster/Boss_Base.h"

// Sets default values
ABossDoor_Base::ABossDoor_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(m_Box);

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_Mesh->SetupAttachment(m_Box);
}

// Called when the game starts or when spawned
void ABossDoor_Base::BeginPlay()
{
	Super::BeginPlay();

	
	m_Box->OnComponentBeginOverlap.AddDynamic(this, &ABossDoor_Base::BeginOverlap);
}

// Called every frame
void ABossDoor_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossDoor_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _Hit)
{
	ACharacter_Base* pCharacter = Cast<ACharacter_Base>(_OtherActor);
	if (pCharacter)
	{
		Log(LogTemp, Warning, TEXT("door 트리거 발생"));
		Cast<ABoss_Base>(Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()))->m_Boss)->SetTarget(pCharacter);
	}
}


