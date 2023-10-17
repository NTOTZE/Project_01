// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSDetectPlayer.h"

#include "AIController.h"
#include "behaviortree/blackboardcomponent.h"
#include "../Boss_Base.h"

UBTSDetectPlayer::UBTSDetectPlayer()
{
	// 서비스 설명 적어주기
	NodeName = TEXT("Detect Player");

	// 서비스 호출 간격 // 0.5초
	Interval = 0.5f;
}

void UBTSDetectPlayer::TickNode(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnCom, _NodeMemory, _DT);

	AAIController* pController = _OwnCom.GetAIOwner();
	if (!IsValid(pController))
		return;
	ABoss_Base* pBoss = Cast<ABoss_Base>(pController->GetPawn());
	if (!IsValid(pBoss))
		return;

	pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), pBoss->GetTarget());
	if (pBoss->GetTarget() == nullptr)
		pController->GetBlackboardComponent()->SetValueAsBool(TEXT("BattleEntarance"), false);
}