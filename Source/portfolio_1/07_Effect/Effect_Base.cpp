// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Base.h"

#include "Particles\ParticleSystemComponent.h"
#include "NiagaraComponent.h"

#include "../00_System/EffectDataAsset.h"

// Sets default values
AEffect_Base::AEffect_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(m_Scene);

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_Particle->SetupAttachment(m_Scene);

	m_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	m_Niagara->SetupAttachment(m_Scene);

	ConstructorHelpers::FObjectFinder<UEffectDataAsset> effectSetting(TEXT("/Script/portfolio_1.EffectDataAsset'/Game/00_BlueprintClass/Effect/DA_GlobalEffect.DA_GlobalEffect'"));
	if (effectSetting.Succeeded())
		EffectSetting = effectSetting.Object;
}

// Called when the game starts or when spawned
void AEffect_Base::BeginPlay()
{
	Super::BeginPlay();

	UEffectDataAsset* pDataAsset = EffectSetting.LoadSynchronous();
	
	m_bNiagaraPlay = false;
	m_bParticlePlay = false;

	if (IsValid(pDataAsset))
	{
		if (!pDataAsset->EffectDataArr[(int32)Type].Particle.IsNull())
		{
			UParticleSystem* pParticle = pDataAsset->EffectDataArr[(int32)Type].Particle.LoadSynchronous();
			if (IsValid(pParticle))
			{
				m_Particle->SetTemplate(pParticle);
				m_Particle->OnSystemFinished.AddDynamic(this, &AEffect_Base::ParticleFinished);
				m_bParticlePlay = true;
			}
		}
		if (!pDataAsset->EffectDataArr[(int32)Type].Niagara.IsNull())
		{
			UNiagaraSystem* pNiagara = pDataAsset->EffectDataArr[(int32)Type].Niagara.LoadSynchronous();
			if (IsValid(pNiagara))
			{
				m_Niagara->SetAsset(pNiagara);
				m_bNiagaraPlay = true;
			}
		}
	}
}

// Called every frame
void AEffect_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(m_Niagara->GetAsset()) && m_Niagara->IsComplete())
	{
		m_bNiagaraPlay = false;
	}

	if (m_bNiagaraPlay == false && m_bParticlePlay == false)
	{
		Destroy();
	}
}

void AEffect_Base::ParticleFinished(UParticleSystemComponent* _PSC)
{
	m_bParticlePlay = false;
}