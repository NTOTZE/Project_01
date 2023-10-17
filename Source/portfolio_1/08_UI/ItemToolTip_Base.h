// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTip_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UItemToolTip_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UImage* m_IconImg;

	UPROPERTY()
	class UTextBlock* m_Name;

	UPROPERTY()
	class UTextBlock* m_Description;

public:
	class UImage* GetIconImageWidget() { return m_IconImg; }
	class UTextBlock* GetNameWidget() { return m_Name; }
	class UTextBlock* GetDescriptionWidget() { return m_Description; }

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
