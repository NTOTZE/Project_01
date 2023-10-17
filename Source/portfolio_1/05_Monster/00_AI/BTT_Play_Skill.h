// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../00_Header/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Play_Skill.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UBTT_Play_Skill : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "BossSkill", meta = (AllowPrivateAccess = "true"))
	EBOSS_SKILL m_Skill;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTT_Play_Skill();
};
