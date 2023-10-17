// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EffectMgr.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UEffectMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void CreateEffect(UWorld* _World, EEFFECT_TYPE _Type, ULevel* _Level, FTransform _Trans);
private:
	UEffectMgr() {}
};
