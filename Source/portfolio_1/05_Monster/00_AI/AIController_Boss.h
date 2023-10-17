// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../00_Header/global.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Boss.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API AAIController_Boss : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY()
	UBlackboardData* m_Blackboard;

	UPROPERTY()
	class UBossSkillDataAsset*	m_SkillData;
public:
	virtual void OnPossess(APawn* _Owner) override;
	virtual void OnUnPossess() override;
	void SetSkillData(class UBossSkillDataAsset* _Data);
	class UBossSkillDataAsset* GetSkillData();
};
