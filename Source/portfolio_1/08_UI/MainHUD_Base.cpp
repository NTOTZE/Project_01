// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD_Base.h"

#include "Blueprint/UserWidget.h"

#include "../00_Header/global.h"
#include "../01_Manager/InvenMgr.h"
#include "BossInfo_Base.h"
#include "PlayerInfo_Base.h"
#include "ItemInteraction_Base.h"
#include "Inventory_Base.h"

void UMainHUD_Base::NativeConstruct()
{
	Super::NativeConstruct();
	m_BossInfo = Cast<UBossInfo_Base>(GetWidgetFromName(TEXT("BossInfo")));
	m_PlayerInfo = Cast<UPlayerInfo_Base>(GetWidgetFromName(TEXT("PlayerInfo")));
	m_ItemInteraction = Cast<UItemInteraction_Base>(GetWidgetFromName(TEXT("ItemInteraction")));
	m_Inventory = Cast<UInventory_Base>(GetWidgetFromName(TEXT("Inventory")));

	m_bKeyDownTrg = false;
	m_InputKey = EKeys::Invalid;

	if (!IsValid(m_BossInfo) || !IsValid(m_PlayerInfo) || !IsValid(m_ItemInteraction) || !IsValid(m_Inventory))
	{
		Log(Log_UI, Error, TEXT("위젯 찾지 못함"));
	}
	else
	{
		m_BossInfo->SetVisibility(ESlateVisibility::Hidden);
		m_ItemInteraction->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUD_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ui only 입력모드 키처리.
	// MainHUD가 포커스된 후 i키를 누르면 CloseInventoryUI가 호출되고
	// CloseInventoryUI는 인벤토리를 닫은 뒤 입력모드를 gameonly로 변경한다.
	// 그런데 NativeOnKeyDown 안에서 처리하면 ui only 모드에서 입력한 i키가
	// gameonly 입력 모드에도 영향을 미쳐서 닫힌 인벤토리가 바로 다시열려 닫히지 않게된다.
	// 그래서 트리거를 받아와 Tick에서 처리하도록 변경했는데, 문제없이 동작하는것을 확인.
	if (m_bKeyDownTrg)
	{
		if (m_InputKey == EKeys::I)
		{
			UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInvenMgr>()->CloseInventoryUI();
		}
		m_bKeyDownTrg = false;
		m_InputKey = EKeys::Invalid;
	}
}

FReply UMainHUD_Base::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	m_bKeyDownTrg = true;
	m_InputKey = InKeyEvent.GetKey();

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UMainHUD_Base::ShowBossInfoWidget(bool _visible)
{
	if(_visible)
	{
		m_BossInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		m_BossInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUD_Base::ShowItemInteractionWidget(bool _visible)
{
	if (_visible)
	{
		m_ItemInteraction->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		m_ItemInteraction->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUD_Base::ShowInventoryUI(bool _visible)
{
	if (_visible)
	{
		m_Inventory->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{	
		m_Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UMainHUD_Base::IsInventoryOpen()
{
	if (ESlateVisibility::Visible == m_Inventory->GetVisibility())
		return true;
	return false;
}
