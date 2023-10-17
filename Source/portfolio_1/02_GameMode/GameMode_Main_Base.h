// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_Main_Base.generated.h"

UCLASS()
class PORTFOLIO_1_API AGameMode_Main_Base : public AGameModeBase
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<AActor> m_Boss;
	TWeakObjectPtr<AActor> m_Player;
	
private:
	UPROPERTY()
	TSubclassOf<class UMainHUD_Base>	m_MainHudClass;
	
	UPROPERTY()
	class UMainHUD_Base* m_MainHUD;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	class UMainHUD_Base* GetMainHUD() { return m_MainHUD; }
	void EnableUIInputMode(bool _b);

public:
	AGameMode_Main_Base();
};
