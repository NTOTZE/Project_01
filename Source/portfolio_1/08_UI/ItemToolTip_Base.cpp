// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTip_Base.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "../00_Header/global.h"

void UItemToolTip_Base::NativeConstruct()
{
	Super::NativeConstruct();
	m_IconImg = Cast<UImage>(GetWidgetFromName(FName("IconImage")));
	m_Name = Cast<UTextBlock>(GetWidgetFromName(FName("Name")));
	m_Description = Cast<UTextBlock>(GetWidgetFromName(FName("Description")));

	if (!IsValid(m_IconImg) || !IsValid(m_Name) || !IsValid(m_Description))
	{
		Log(Log_UI, Error, TEXT("위젯 찾지 못함"));
	}
}

void UItemToolTip_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
