// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BossSkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UBossSkillDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (DisplayName = "Boss Skill Data"))
	FBossSkillInfo	SkillDataArr[(int32)EBOSS_SKILL::ENUM_END];
};
