// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvenItem_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UInvenItem_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UImage* m_IconImg;
	
	UPROPERTY()
	class UTextBlock* m_ItemCount;

public:
	UFUNCTION(BlueprintCallable)
	void InitFromData(UObject* _Data);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
