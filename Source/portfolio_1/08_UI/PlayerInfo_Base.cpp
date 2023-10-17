// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfo_Base.h"

#include "Components/ProgressBar.h"
#include "../00_Header/global.h"

void UPlayerInfo_Base::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar.Widget = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_MPBar.Widget = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPBar")));
	m_StaminaBar.Widget = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));
	if (!IsValid(m_HPBar.Widget) || !IsValid(m_MPBar.Widget) || !IsValid(m_StaminaBar.Widget))
	{
		Log(Log_UI, Error, TEXT("위젯 찾지 못함"));
	}
	else
	{
		m_HPBar.Widget->SetPercent(m_HPBar.Ratio);
		m_MPBar.Widget->SetPercent(m_MPBar.Ratio);
		m_StaminaBar.Widget->SetPercent(m_StaminaBar.Ratio);
	}
}

void UPlayerInfo_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetRatioSmoothing(m_HPBar, InDeltaTime);
	SetRatioSmoothing(m_MPBar, InDeltaTime);
	SetRatioSmoothing(m_StaminaBar, InDeltaTime);
}

void UPlayerInfo_Base::SetRatioSmoothing(const FStatsBar& _Bar, float _DeltaTime)
{
	const float fMinRate = Temp_MinRateCoeff * _DeltaTime;

	if (_Bar.Widget->GetPercent() != _Bar.Ratio)
	{
		const float fCurRatio = _Bar.Widget->GetPercent();
		const float fRatioDiff = _Bar.Ratio - fCurRatio;
		if (FMath::Abs(fRatioDiff) < fMinRate)
		{
			_Bar.Widget->SetPercent(_Bar.Ratio);
		}
		else
		{
			const float FrameDiffValue = fRatioDiff * Temp_FrameDiffCoeff * _DeltaTime;
			if (FrameDiffValue > 0.f)
				_Bar.Widget->SetPercent(fCurRatio + FrameDiffValue + fMinRate);
			else
				_Bar.Widget->SetPercent(fCurRatio + FrameDiffValue - fMinRate);
		}
	}
}

void UPlayerInfo_Base::SetHPRatio(float _Ratio)
{
	m_HPBar.Ratio = _Ratio;
}

void UPlayerInfo_Base::SetMPRatio(float _Ratio)
{
	m_MPBar.Ratio = _Ratio;
}

void UPlayerInfo_Base::SetStaminaRatio(float _Ratio)
{
	m_StaminaBar.Ratio = _Ratio;
}