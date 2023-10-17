// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Base.h"

#include "Components/TileView.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "../00_Header/global.h"
#include "../00_System/InvenItemObject.h"
#include "ItemToolTip_Base.h"

void UInventory_Base::NativeConstruct()
{
	Super::NativeConstruct();
	m_TileView = Cast<UTileView>(GetWidgetFromName(TEXT("TileView")));
	if (!IsValid(m_TileView))
	{
		Log(LogTemp, Error, TEXT("Inventory 위젯에서 TileView 찾기 실패"))
	}
	else
	{
		UBlueprintGeneratedClass* TooltipClass = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/00_BlueprintClass/UI/HUD/WBP_ItemDescription.WBP_ItemDescription_C'"));
		if (IsValid(TooltipClass))
		{
			m_TileView->SetToolTip(CreateWidget(GetWorld(), TooltipClass));
			Log(Log_UI, Warning, TEXT("OnHovered Binding"));
			m_TileView->OnItemIsHoveredChanged().AddUObject(this, &UInventory_Base::OnHovered);
			m_TileView->GetToolTip()->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			Log(Log_UI, Warning, TEXT("OnHovered Binding Failed"));
		}
	}
}

void UInventory_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventory_Base::OnHovered(UObject* _Data, bool _hovered)
{
	UInvenItemObject* pItem = Cast<UInvenItemObject>(_Data);
	Log(Log_UI, Warning, TEXT("OnHovered"));
	if (_hovered)
	{
		UItemToolTip_Base* pTooltip = Cast<UItemToolTip_Base>(m_TileView->ToolTipWidget);

		FString IconPath = pItem->GetIconImgPath();
		UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *IconPath);
		pTooltip->GetIconImageWidget()->SetBrushFromTexture(pTex2D);
		pTooltip->GetNameWidget()->SetText(FText::FromString(pItem->GetItemName()));
		pTooltip->GetDescriptionWidget()->SetText(FText::FromString(pItem->GetItemDescription()));

		m_TileView->GetToolTip()->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_TileView->GetToolTip()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventory_Base::AddItem(UObject* _ItemData)
{
	m_TileView->AddItem(_ItemData);
}

void UInventory_Base::Clear()
{
	m_TileView->ClearListItems();
}
