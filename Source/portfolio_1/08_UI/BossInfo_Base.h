// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossInfo_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UBossInfo_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UTextBlock* m_Name;
	
	UPROPERTY()
	class UProgressBar* m_HPBar;

	FText	m_NameSet;
	float	m_Ratio;

	const float Temp_FrameDiffCoeff = 2.5f;
	const float Temp_MinRateCoeff = 0.01f;

public:
	void SetHPRatio(float _Ratio);
	void SetTextBlock(const FString& _Name);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
