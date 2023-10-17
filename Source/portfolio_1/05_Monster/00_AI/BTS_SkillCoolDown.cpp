// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SkillCoolDown.h"

#include "AIController.h"
#include "AIController_Boss.h"
#include "behaviortree/blackboardcomponent.h"
#include "../../00_System/BossSkillDataAsset.h"
#include "../../00_Header/global.h"

UBTS_SkillCoolDown::UBTS_SkillCoolDown()
{
	// 서비스 설명 적어주기
	NodeName = TEXT("SkillCoolDown");

	// 서비스 호출 간격 // 0.5초
	Interval = 0.1f;
}

void UBTS_SkillCoolDown::TickNode(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnCom, _NodeMemory, _DT);
	AAIController* pController = Cast<AAIController>(_OwnCom.GetAIOwner());
	AAIController_Boss* pBossController = Cast<AAIController_Boss>(pController);
	if (!pBossController)
	{
		Log(Log_AI, Error, TEXT(""));
	}
	UBossSkillDataAsset* pDA = pBossController->GetSkillData();

	//UBossSkillDataAsset* pDA = Cast<UBossSkillDataAsset>(pController->GetBlackboardComponent()->GetValueAsObject(TEXT("SkillData")));
	
	if (!IsValid(pDA))
	{
		Log(LogTemp, Error, TEXT("Boss Skill Data Asset 못찾음"));
		return;
	}


	for (int32 i = 0; i < (int32)EBOSS_SKILL::ENUM_END; ++i)
	{
		if (pDA->SkillDataArr[i].CurCoolTime <= _DT)
			pDA->SkillDataArr[i].CurCoolTime = 0.f;
		else
			pDA->SkillDataArr[i].CurCoolTime -= _DT;
	}

}
