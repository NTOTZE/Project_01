// Fill out your copyright notice in the Description page of Project Settings.


#include "BossInfo_Base.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "../00_Header/global.h"

void UBossInfo_Base::NativeConstruct()
{
	Super::NativeConstruct();
	m_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	if (!IsValid(m_Name) || !IsValid(m_HPBar))
	{
		Log(Log_UI, Error, TEXT("위젯 찾지 못함"));
	}
	else
	{
		m_Name->SetText(m_NameSet);
		m_HPBar->SetPercent(m_Ratio);
	}
}

void UBossInfo_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float fMinRate = Temp_MinRateCoeff * InDeltaTime;

	if (m_HPBar->GetPercent() != m_Ratio)
	{
		const float fCurRatio = m_HPBar->GetPercent();
		const float fRatioDiff = m_Ratio - fCurRatio;
		if (FMath::Abs(fRatioDiff) < fMinRate)
		{
			m_HPBar->SetPercent(m_Ratio);
		}
		else
		{
			const float FrameDiffValue = fRatioDiff * Temp_FrameDiffCoeff * InDeltaTime;
			
			if(FrameDiffValue > 0.f)
				m_HPBar->SetPercent(fCurRatio + FrameDiffValue + fMinRate);
			else
				m_HPBar->SetPercent(fCurRatio + FrameDiffValue - fMinRate);
		}
	}
}

void UBossInfo_Base::SetHPRatio(float _Ratio)
{
	m_Ratio = _Ratio;
}

void UBossInfo_Base::SetTextBlock(const FString& _Name)
{
	m_NameSet = FText::FromString(_Name);
	if (IsValid(m_Name))
	{
		m_Name->SetText(m_NameSet);
	}
}
