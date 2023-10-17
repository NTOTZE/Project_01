// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EffectDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UEffectDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Effect", meta = (DisplayName = "Global Effect Data"))
	FEffectData	EffectDataArr[(int32)EEFFECT_TYPE::ENUM_END];
};
