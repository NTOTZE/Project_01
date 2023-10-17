// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BattleEntrance.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UBTT_BattleEntrance : public UBTTaskNode
{
	GENERATED_BODY()

private:
	//스킬 데이터 에셋
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<class UBossSkillDataAsset> m_SkillDataAsset;

	//등장 애니메이션 몽타주
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UAnimMontage> m_Montage;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTT_BattleEntrance();
};
