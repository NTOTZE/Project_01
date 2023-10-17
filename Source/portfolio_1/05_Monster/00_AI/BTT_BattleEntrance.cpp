// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BattleEntrance.h"

#include "AIController.h"
#include "AIController_Boss.h"
#include "behaviortree/blackboardcomponent.h"

#include "../../02_GameMode/GameMode_Main_Base.h"
#include "../../00_System/BossSkillDataAsset.h"
#include "../Boss_Base.h"
#include "../../08_UI/MainHUD_Base.h"
#include "../../08_UI/BossInfo_Base.h"

UBTT_BattleEntrance::UBTT_BattleEntrance()
{
	//틱활성화
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_BattleEntrance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* pController = Cast<AAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(pController))
		return EBTNodeResult::Failed;

	if (pController->GetBlackboardComponent()->GetValueAsBool(TEXT("BattleEntarance")) == true)
		return EBTNodeResult::Succeeded;

	if (m_Montage.IsNull())
		return EBTNodeResult::Failed;


	ABoss_Base* pBoss = Cast<ABoss_Base>(pController->GetPawn());
	if (!IsValid(pBoss))
		return EBTNodeResult::Failed;

	if(m_SkillDataAsset.IsNull())
		return EBTNodeResult::Failed;

	UBossSkillDataAsset* pSkill = m_SkillDataAsset.LoadSynchronous();
	if(!IsValid(pSkill))
		return EBTNodeResult::Failed;
	pController->GetBlackboardComponent()->SetValueAsObject(TEXT("SkillData"), pSkill);
	pController->GetBlackboardComponent()->SetValueAsBool(TEXT("bDeath"), false);

	AAIController_Boss* pBossCon = Cast<AAIController_Boss>(pController);
	if (pBossCon)
	{
		pBossCon->SetSkillData(pSkill);
	}

	pBoss->GetMesh()->GetAnimInstance()->Montage_Play(m_Montage.LoadSynchronous());
	pBoss->SetPlayingMontage(true);

	return EBTNodeResult::InProgress;
}

void UBTT_BattleEntrance::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* pController = Cast<AAIController>(OwnerComp.GetAIOwner());
	ABoss_Base* pBoss = Cast<ABoss_Base>(pController->GetPawn());
	if (pBoss->GetPlayingMontage() == false)
	{
		pBoss->BattleEntrance();
		pController->GetBlackboardComponent()->SetValueAsBool(TEXT("BattleEntarance"), pBoss->GetBattleEntrance());
		pController->GetBlackboardComponent()->SetValueAsBool(TEXT("bDeath"), false);
		pController->GetBlackboardComponent()->SetValueAsBool(TEXT("bStagger"), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

}

