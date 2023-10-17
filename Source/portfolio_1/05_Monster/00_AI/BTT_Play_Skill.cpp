// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Play_Skill.h"

#include "AIController.h"
#include "AIController_Boss.h"
#include "behaviortree/blackboardcomponent.h"
#include "../../00_System/BossSkillDataAsset.h"
#include "../Boss_Base.h"

UBTT_Play_Skill::UBTT_Play_Skill()
{
	//틱활성화
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Play_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* pController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(pController))
		return EBTNodeResult::Failed;

	//UBossSkillDataAsset* pDA = Cast<UBossSkillDataAsset>(pController->GetBlackboardComponent()->GetValueAsObject(TEXT("SkillData")));
	AAIController_Boss* pBossController = Cast<AAIController_Boss>(pController);
	if(!pBossController)
		return EBTNodeResult::Failed;

	UBossSkillDataAsset* pDA = pBossController->GetSkillData();
	if(!IsValid(pDA))
		return EBTNodeResult::Failed;

	if((int32)m_Skill >= (int32)EBOSS_SKILL::ENUM_END )
		return EBTNodeResult::Failed;

	if (pDA->SkillDataArr[(int32)m_Skill].Montage.IsNull())
		return EBTNodeResult::Failed;

	UAnimMontage* pMontage = pDA->SkillDataArr[(int32)m_Skill].Montage.LoadSynchronous();
	if(!IsValid(pMontage))
		return EBTNodeResult::Failed;

	ABoss_Base* pBoss = Cast<ABoss_Base>(pController->GetPawn());
	if (!IsValid(pBoss))
		return EBTNodeResult::Failed;

	// 스킬 쿨타임 판단
	if(pDA->SkillDataArr[(int32)m_Skill].CurCoolTime > 0.f)
		return EBTNodeResult::Succeeded;

	float fDist = FVector::Distance(pBoss->GetTarget()->GetActorLocation(), pBoss->GetActorLocation());
	// 스킬 최대 발동 거리
	if (pDA->SkillDataArr[(int32)m_Skill].MaxRange > 0.f)
	{
		if(fDist > pDA->SkillDataArr[(int32)m_Skill].MaxRange)
			return EBTNodeResult::Succeeded;
	}
	
	// 스킬 최소 발동 거리
	if (pDA->SkillDataArr[(int32)m_Skill].MinRange > 0.f)
	{
		if (fDist < pDA->SkillDataArr[(int32)m_Skill].MinRange)
			return EBTNodeResult::Succeeded;
	}

	// 스킬 발동 확률
	if(FMath::RandRange(1, 100) > pDA->SkillDataArr[(int32)m_Skill].Probability)
		return EBTNodeResult::Succeeded;

	pBoss->GetMesh()->GetAnimInstance()->Montage_Play(pMontage);
	pBoss->SetPlayingMontage(true);

	return EBTNodeResult::InProgress;
}

void UBTT_Play_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* pController = Cast<AAIController>(OwnerComp.GetAIOwner());
	ABoss_Base* pBoss = Cast<ABoss_Base>(pController->GetPawn());

	// 임시코드. hp 0이하면 몽타주 멈추고 Death 몽타주 출력
	if (pBoss->GetCurrentStats().HP <= 0.f)
	{
		if (false == pController->GetBlackboardComponent()->GetValueAsBool(TEXT("bDeath")))
		{
			pController->GetBlackboardComponent()->SetValueAsBool(TEXT("bDeath"), true);
			pBoss->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);

			AAIController_Boss* pBossController = Cast<AAIController_Boss>(pController);
			if (!pBossController)
			{
				Log(Log_AI, Error, TEXT(""));
			}
			UBossSkillDataAsset* pDA = pBossController->GetSkillData();

			//UBossSkillDataAsset* pDA = Cast<UBossSkillDataAsset>(pController->GetBlackboardComponent()->GetValueAsObject(TEXT("SkillData")));
			pController->GetBlackboardComponent()->GetValueAsBool(TEXT("bDeath"));
			UAnimMontage* pMontage = pDA->SkillDataArr[(int32)EBOSS_SKILL::DEATH].Montage.LoadSynchronous();
			if (IsValid(pMontage))
			{
				pBoss->GetMesh()->GetAnimInstance()->Montage_Play(pMontage);
			}
		}

	}
	else if (pBoss->GetPlayingMontage() == false)
	{
		AAIController_Boss* pBossController = Cast<AAIController_Boss>(pController);
		if (!pBossController)
		{
			Log(Log_AI, Error, TEXT(""));
		}
		UBossSkillDataAsset* pDA = pBossController->GetSkillData();

		//UBossSkillDataAsset* pDA = Cast<UBossSkillDataAsset>(pController->GetBlackboardComponent()->GetValueAsObject(TEXT("SkillData")));
		pDA->SkillDataArr[(int32)m_Skill].CurCoolTime = pDA->SkillDataArr[(int32)m_Skill].MaxCoolTime;

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
