// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Effect_Base.generated.h"

UCLASS()
class PORTFOLIO_1_API AEffect_Base : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	USceneComponent* m_Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	UParticleSystemComponent* m_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	class UNiagaraComponent* m_Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	EEFFECT_TYPE	Type;

	UPROPERTY(EditAnywhere, Category = "Component")
	TSoftObjectPtr<class UEffectDataAsset> EffectSetting;

	bool	m_bNiagaraPlay;
	bool	m_bParticlePlay;

public:
	void SetEffectType(EEFFECT_TYPE _Type) { Type = _Type; }

public:
	UFUNCTION()
	void ParticleFinished(UParticleSystemComponent* _PSC);

public:	
	// Sets default values for this actor's properties
	AEffect_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
