// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo_Base.generated.h"

USTRUCT()
struct FStatsBar
{
	GENERATED_BODY()

	UPROPERTY()
	class UProgressBar* Widget;
	
	float	Ratio;
};

UCLASS()
class PORTFOLIO_1_API UPlayerInfo_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	FStatsBar m_HPBar;
	FStatsBar m_MPBar;
	FStatsBar m_StaminaBar;

	const float Temp_FrameDiffCoeff = 2.5f;
	const float Temp_MinRateCoeff = 0.01f;

	void SetRatioSmoothing(const FStatsBar& _Bar, float _DeltaTime);

public:
	void SetHPRatio(float _Ratio);
	void SetMPRatio(float _Ratio);
	void SetStaminaRatio(float _Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
