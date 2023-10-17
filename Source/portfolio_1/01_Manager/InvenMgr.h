// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InvenMgr.generated.h"

struct FItemStack
{
	FGameItemInfo*	ItemInfo;
	int32			stack;
};

UCLASS()
class PORTFOLIO_1_API UInvenMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class UDataTable* m_ItemDataTable;

	TMap<EITEM_ID, FGameItemInfo>	m_mapItemInfo;
	TMap<EITEM_ID, FItemStack>		m_BackPack[(int32)EITEM_TYPE::ENUM_END];

public:
	void AddGameItem(EITEM_ID _ID);

	void OpenInventoryUI();
	void CloseInventoryUI();
private:
	void ResetWidget();

private:
	UInvenMgr();
};
