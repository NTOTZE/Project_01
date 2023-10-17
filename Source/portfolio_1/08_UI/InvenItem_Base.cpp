// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenItem_Base.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "../00_Header/global.h"
#include "../00_System/InvenItemObject.h"

void UInvenItem_Base::NativeConstruct()
{
	Super::NativeConstruct();

	m_IconImg = Cast<UImage>(GetWidgetFromName(TEXT("IconImage")));
	m_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));

	if (!IsValid(m_IconImg) || !IsValid(m_ItemCount))
	{
		Log(LogTemp, Error, TEXT("Inventory Item 위젯에서 하위 위젯 찾기 실패"))
	}
}

void UInvenItem_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInvenItem_Base::InitFromData(UObject* _Data)
{
	UInvenItemObject* pItemObject = Cast<UInvenItemObject>(_Data);
	if (!IsValid(pItemObject))
	{
		Log(LogTemp, Error, TEXT("Inventory 위젯 입력 데이터가 올바르지 않습니다."));
		return;
	}

	FString IconPath = pItemObject->GetIconImgPath();
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *IconPath);
	m_IconImg->SetBrushFromTexture(pTex2D);
	m_ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), pItemObject->GetItemCount())));
}
