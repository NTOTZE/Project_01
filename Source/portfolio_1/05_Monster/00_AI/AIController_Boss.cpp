// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Boss.h"
#include "../../00_Header/global.h"
#include "../../00_System/BossSkillDataAsset.h"
#include "../Boss_Base.h"

void AAIController_Boss::OnPossess(APawn* _Owner)
{
	Super::OnPossess(_Owner);

	//빙의한 Pawn이 Monster_Base의 파생클래스인지
	ABoss_Base* pBoss = Cast<ABoss_Base>(_Owner);
	
	if (!IsValid(pBoss))
	{
		Log(Log_AI, Error, TEXT("컨트롤러 빙의 대상 오류 : 몬스터가 아님"));
		return;
	}

	// 빙의 대상(몬스터) 로 부터, 사용할 행동트리를 가져온다.
	m_BehaviorTree = pBoss->GetBehaviorTree();
	m_Blackboard = pBoss->GetBlackboard();

	if (IsValid(m_Blackboard))
	{
		UBlackboardComponent* pCom = nullptr;
		UseBlackboard(m_Blackboard, pCom);
		if (IsValid(m_BehaviorTree))
		{
			RunBehaviorTree(m_BehaviorTree);
		}
	}
}

void AAIController_Boss::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAIController_Boss::SetSkillData(UBossSkillDataAsset* _Data)
{ 
	m_SkillData = _Data;
}

UBossSkillDataAsset* AAIController_Boss::GetSkillData()
{
	return m_SkillData;
}
