// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponComboDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UWeaponComboDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Combo", meta = (DisplayName = "WeaponComboData"))
	FWeaponAttackData	ComboDataArr[(int32)EWeaponAttackType::ENUM_END];
};
