// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Main_Base.h"

#include "../00_Header/global.h"
#include "../03_Character/Character_Base.h"
#include "../05_Monster/Boss_Base.h"
#include "../08_UI/MainHUD_Base.h"
#include "../08_UI/PlayerInfo_Base.h"
#include "../08_UI/BossInfo_Base.h"

AGameMode_Main_Base::AGameMode_Main_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UMainHUD_Base> Finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/00_BlueprintClass/UI/HUD/WBP_MainHUD.WBP_MainHUD_C'"));
	if (Finder.Succeeded())
	{
		m_MainHudClass = Finder.Class;
	}
}

void AGameMode_Main_Base::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_MainHudClass))
	{
		UMainHUD_Base* pHUD = Cast<UMainHUD_Base>(CreateWidget(GetWorld(), m_MainHudClass));
		if (pHUD)
		{
			m_MainHUD = pHUD;
			if (!IsValid(m_MainHUD))
			{
				Log(Log_UI, Error, TEXT("MainHUD 유효하지 않음"));
			}
			else
			{
				m_MainHUD->AddToViewport();
			}
		}
		else
		{
			Log(Log_UI, Error, TEXT("MainHUDClass 캐스팅 실패"));
		}
	}
	else
	{
		Log(Log_UI, Error, TEXT("MainHUDClass 유효하지 않음"));
	}

}

void AGameMode_Main_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsValid(m_MainHUD))
	{
		ACharacter_Base* pPlayer = Cast<ACharacter_Base>(m_Player.Get());
		if (pPlayer)
		{
			m_MainHUD->GetPlayerInfoWidget()
				->SetHPRatio(pPlayer->GetCurrentStats().HP / pPlayer->GetDefaultStats().HP);

			m_MainHUD->GetPlayerInfoWidget()
				->SetMPRatio(pPlayer->GetCurrentStats().MP / pPlayer->GetDefaultStats().MP);

			m_MainHUD->GetPlayerInfoWidget()
				->SetStaminaRatio(pPlayer->GetCurrentStats().Stamina / pPlayer->GetDefaultStats().Stamina);
		}

		ABoss_Base* pBoss = Cast<ABoss_Base>(m_Boss);
		if (pBoss)
		{
			m_MainHUD->GetBossInfoWidget()
				->SetHPRatio(pBoss->GetCurrentStats().HP / pBoss->GetDefaultStats().HP);
		}

	}
}

void AGameMode_Main_Base::EnableUIInputMode(bool _b)
{
	APlayerController* pController = GetWorld()->GetFirstPlayerController();
	if (_b)
	{
		FInputModeUIOnly uionly;
		pController->SetInputMode(uionly);
		m_MainHUD->SetFocus();
		pController->bShowMouseCursor = true;
	}
	else
	{
		FInputModeGameOnly gameonly;
		pController->SetInputMode(gameonly);
		pController->bShowMouseCursor = false;
	}

}

