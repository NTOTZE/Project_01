// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IADataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UIADataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "CharacterInputAction"))
	TSoftObjectPtr<UInputAction>	CIADataArr[(int32)ECharacterInputActionType::ENUM_END];

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "WeaponInputAction"))
	TSoftObjectPtr<UInputAction>	WIADataArr[(int32)EWeaponInputActionType::ENUM_END];
};
