// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectMgr.h"

#include "../07_Effect/Effect_Base.h"

void UEffectMgr::CreateEffect(UWorld* _World, EEFFECT_TYPE _Type, ULevel* _Level, FTransform _Trans)
{
	//스폰할떄 파라메터 넘겨줄수있음
	FActorSpawnParameters param = {};
	//스폰된 액터가 충돌발생시 어떻게 할 것인가 = AlwaysSpawn 그냥 해당 위치에 스폰
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//엑터를 스폰시킬 레벨 설정 = 현재 Character_Base의 레벨
	param.OverrideLevel = _Level;
	//지연생성 여부 = true(beginplay가 바로 호출되지않음)
	param.bDeferConstruction = true;

	//StaticClass는 실제 객체가 아닌 UClass의 주소를 불러오며
	//지정된 UClass(StaticClass)는 SpawnActor탬플릿의 타입<AEffect_Base>의 자손이어야한다.
	AEffect_Base* pEffect = _World->SpawnActor<AEffect_Base>(AEffect_Base::StaticClass(), _Trans, param);
	pEffect->SetEffectType(_Type);

	pEffect->FinishSpawning(pEffect->GetTransform());
}

