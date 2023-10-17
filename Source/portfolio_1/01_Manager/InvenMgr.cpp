// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenMgr.h"

#include "../00_System/InvenItemObject.h"
#include "../02_GameMode/GameMode_Main_Base.h"
#include "../08_UI/MainHUD_Base.h"
#include "../08_UI/Inventory_Base.h"
#include "../08_UI/InvenItem_Base.h"

UInvenMgr::UInvenMgr()
{
	ConstructorHelpers::FObjectFinder<UDataTable> TableFinder(TEXT("/Script/Engine.DataTable'/Game/00_BlueprintClass/Table/DT_ItemInfo.DT_ItemInfo'"));
	if (TableFinder.Succeeded())
	{
		m_ItemDataTable = TableFinder.Object;
	}
	else
	{
		Log(LogTemp, Error, TEXT("아이템 데이터 테이블 찾지 못함"));
	}

	if (!IsValid(m_ItemDataTable))
	{
		Log(LogTemp, Error, TEXT("아이템 데이터 테이블 유효하지 않음"));
	}
	else
	{
		FString str;
		TArray<FGameItemInfo*> arrTableData;
		m_ItemDataTable->GetAllRows<FGameItemInfo>(str, arrTableData);

		for (int32 i = 0; i < arrTableData.Num(); ++i)
		{
			m_mapItemInfo.Add(arrTableData[i]->ID, *arrTableData[i]);
		}
	}
}

void UInvenMgr::AddGameItem(EITEM_ID _ID)
{
	FGameItemInfo* pItemInfo = m_mapItemInfo.Find(_ID);
	if (nullptr == pItemInfo)
	{
		Log(LogTemp, Error, TEXT("ITEM ID에 해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	FItemStack* fItemStack = m_BackPack[(int32)pItemInfo->TYPE].Find(_ID);
	if (nullptr == fItemStack)
	{
		m_BackPack[(int32)pItemInfo->TYPE].Add(_ID, FItemStack{ pItemInfo, 1 });
	}
	else
	{
		++fItemStack->stack;
	}

	AGameMode_Main_Base* pGamemode = Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(pGamemode))
		return;

	UMainHUD_Base* pMainHUD = pGamemode->GetMainHUD();

	if (pMainHUD->IsInventoryOpen())
		ResetWidget();
}

void UInvenMgr::OpenInventoryUI()
{
	AGameMode_Main_Base* pGamemode = Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(pGamemode))
		return;

	UMainHUD_Base* pMainHUD = pGamemode->GetMainHUD();

	ResetWidget();

	//InventoryWidget Visible
	pMainHUD->ShowInventoryUI(true);

	pGamemode->EnableUIInputMode(true);
}

void UInvenMgr::CloseInventoryUI()
{
	AGameMode_Main_Base* pGamemode = Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(pGamemode))
		return;

	UMainHUD_Base* pMainHUD = pGamemode->GetMainHUD();
	UInventory_Base* pInvenWidget = pMainHUD->GetInventoryWidget();

	pInvenWidget->Clear();
	pMainHUD->ShowInventoryUI(false);

	pGamemode->EnableUIInputMode(false);
}

void UInvenMgr::ResetWidget()
{
	AGameMode_Main_Base* pGamemode = Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(pGamemode))
		return;

	UMainHUD_Base* pMainHUD = pGamemode->GetMainHUD();
	UInventory_Base* pInvenWidget = pMainHUD->GetInventoryWidget();

	// InventoryWidget(UI) 이전 내용 클리어
	pInvenWidget->Clear();

	for (int32 i = 0; i < (int32)EITEM_TYPE::ENUM_END; ++i)
	{
		for (auto Iterator = m_BackPack[i].CreateConstIterator(); Iterator; ++Iterator)
		{
			UInvenItemObject* pItemObject = NewObject<UInvenItemObject>();

			pItemObject->SetIconImgPath(Iterator.Value().ItemInfo->IconPath);
			pItemObject->SetItemName(Iterator.Value().ItemInfo->Name);
			pItemObject->SetItemDescription(Iterator.Value().ItemInfo->Description);
			pItemObject->SetItemCount(Iterator.Value().stack);
			pInvenWidget->AddItem(pItemObject);
		}
	}
}
