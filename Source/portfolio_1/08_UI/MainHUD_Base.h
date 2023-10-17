// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UMainHUD_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UBossInfo_Base*			m_BossInfo;

	UPROPERTY()
	class UPlayerInfo_Base*			m_PlayerInfo;

	UPROPERTY()
	class UItemInteraction_Base*	m_ItemInteraction;

	UPROPERTY()
	class UInventory_Base*			m_Inventory;

	bool m_bKeyDownTrg;
	FKey m_InputKey;

public:
	class UBossInfo_Base* GetBossInfoWidget() { return m_BossInfo; }
	class UPlayerInfo_Base* GetPlayerInfoWidget() { return m_PlayerInfo; }
	class UItemInteraction_Base* GetItemInteractionWidget() { return m_ItemInteraction; }
	class UInventory_Base* GetInventoryWidget() { return m_Inventory; }

	void ShowBossInfoWidget(bool _visible);
	void ShowItemInteractionWidget(bool _visible);
	void ShowInventoryUI(bool _visible);

	bool IsInventoryOpen();

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
