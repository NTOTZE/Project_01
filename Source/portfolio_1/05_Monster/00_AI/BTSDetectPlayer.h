// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSDetectPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UBTSDetectPlayer : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMemory, float _DT) override;

public:
	UBTSDetectPlayer();
};
