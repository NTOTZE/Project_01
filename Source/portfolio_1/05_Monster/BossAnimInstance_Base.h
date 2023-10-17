// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UBossAnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<class ABoss_Base> m_Boss;

public:
	//런타임 초기에 한번
	virtual void NativeInitializeAnimation() override;
	//월드 생성될때 호출
	virtual void NativeBeginPlay() override;
	//틱마다 호출
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

private:
	UFUNCTION()
	void AnimNotify_Mont_End();

	UFUNCTION()
	void AnimNotify_Weapon_Start();

	UFUNCTION()
	void AnimNotify_Weapon_End();

	UFUNCTION()
	void AnimNotify_Attack_TraceCh();

	UFUNCTION()
	void AnimNotify_LeftArm_Start();

	UFUNCTION()
	void AnimNotify_LeftArm_End(); 

	UFUNCTION()
	void AnimNotify_Effect_Start();
	
	UFUNCTION()
	void AnimNotify_Effect_End();

	UFUNCTION()
	void AnimNotify_LookAt_Start();

	UFUNCTION()
	void AnimNotify_LookAt_End();

	UFUNCTION()
	void AnimNotify_Died();
};
