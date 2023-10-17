// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance_Base.h"

#include "NiagaraComponent.h"

#include "../05_Monster/Boss_Base.h"


void UBossAnimInstance_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	m_Boss = Cast<ABoss_Base>(TryGetPawnOwner());
}

void UBossAnimInstance_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UBossAnimInstance_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);
}

void UBossAnimInstance_Base::AnimNotify_Mont_End()
{
	m_Boss.Get()->SetPlayingMontage(false);
}

void UBossAnimInstance_Base::AnimNotify_Weapon_Start()
{
	m_Boss.Get()->SetWeaponCollisionEnable(true);
}

void UBossAnimInstance_Base::AnimNotify_Weapon_End()
{
	m_Boss.Get()->SetWeaponCollisionEnable(false);
}

void UBossAnimInstance_Base::AnimNotify_Attack_TraceCh()
{
}

void UBossAnimInstance_Base::AnimNotify_LeftArm_Start()
{
	m_Boss.Get()->SetLeftArmCollisionEnable(true);
}

void UBossAnimInstance_Base::AnimNotify_LeftArm_End()
{
	m_Boss.Get()->SetLeftArmCollisionEnable(false);
}

void UBossAnimInstance_Base::AnimNotify_Effect_Start()
{
	m_Boss.Get()->GetNiagara()->Activate();
}

void UBossAnimInstance_Base::AnimNotify_Effect_End()
{
	m_Boss.Get()->GetNiagara()->Deactivate();
}

void UBossAnimInstance_Base::AnimNotify_LookAt_Start()
{
	m_Boss.Get()->EnableLookAtTarget(true);
}

void UBossAnimInstance_Base::AnimNotify_LookAt_End()
{
	m_Boss.Get()->EnableLookAtTarget(false);
}

void UBossAnimInstance_Base::AnimNotify_Died()
{
	m_Boss.Get()->Died();
}