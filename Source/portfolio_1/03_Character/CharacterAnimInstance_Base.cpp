// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance_Base.h"

#include "NiagaraComponent.h"

#include "../04_Weapon/EquipWeapon_Base.h"
#include "../04_Weapon/EquipScabbard_Base.h"

void UCharacterAnimInstance_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UCharacterAnimInstance_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	m_Character = Cast<ACharacter_Base>(GetOwningActor());
	if (IsValid(m_Character))
	{
		m_Movement = m_Character->GetCharacterMovement();
	}
}

void UCharacterAnimInstance_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);

	if (!IsValid(m_Character) || !IsValid(m_Movement))
		return;

////////////////////////////////
// 아직까지 불필요
	m_LocalVelocity = m_Character->GetLocalVelocity();

	m_MoveSpeed = m_LocalVelocity.Size2D();

	m_IsMove = (m_MoveSpeed > 0.f);

	m_IsGround = m_Character->IsGround();

	m_HasAccel = (m_Character->GetMoveInput().Size() > 0.f);
////////////////////////////////

	m_DistanceTraveled = m_Character->GetDistanceTraveled();

	m_MoveAnimState = m_Character->GetMoveType();

	m_MoveAngle = m_Character->GetMoveAngle();

	m_MoveDirection = m_Character->GetMoveDirection();

	m_EquipAnimState = m_Character->GetEquipType();
}

void UCharacterAnimInstance_Base::AnimNotify_Katana_Attach_1()
{
	Log(Log_Player, Warning, TEXT(""));
	m_Character->GetWeapon()->AttachToComponent(m_Character->GetMesh()
		, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("katana_1")));
}

void UCharacterAnimInstance_Base::AnimNotify_Katana_Attach_2()
{
	Log(Log_Player, Warning, TEXT(""));
	m_Character->GetWeapon()->AttachToComponent(m_Character->GetMesh()
		, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("katana_2")));
}

void UCharacterAnimInstance_Base::AnimNotify_Katana_Attach_3()
{
	Log(Log_Player, Warning, TEXT(""));
	m_Character->GetWeapon()->AttachToComponent(m_Character->GetMesh()
		, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("katana_3")));
}

void UCharacterAnimInstance_Base::AnimNotify_Scabbard_Attach_1()
{
	Log(Log_Player, Warning, TEXT(""));
	m_Character->GetWeapon()->GetScabbard()->AttachToComponent(m_Character->GetMesh()
		, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("scabbard_1")));
}

void UCharacterAnimInstance_Base::AnimNotify_Scabbard_Attach_2()
{
	Log(Log_Player, Warning, TEXT(""));
	m_Character->GetWeapon()->GetScabbard()->AttachToComponent(m_Character->GetMesh()
		, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("scabbard_2")));
}

void UCharacterAnimInstance_Base::AnimNotify_Mont_End()
{
	Log(Log_Player, Warning, TEXT(""));
	m_Character->GetWeapon()->ComboClear();
}

void UCharacterAnimInstance_Base::AnimNotify_Attackable()
{
	Log(Log_Player, Warning, TEXT(""));
	m_Character->GetWeapon()->EnableAttack();
}

void UCharacterAnimInstance_Base::AnimNotify_PreInputEnable()
{
	m_Character->GetWeapon()->PreInputEnable();
}

void UCharacterAnimInstance_Base::AnimNotify_Equip_Weapon()
{
	m_Character->SetEquipType(EWeaponEquipStateType::EQUIP);
}

void UCharacterAnimInstance_Base::AnimNotify_Unequip_Weapon()
{
	m_Character->SetEquipType(EWeaponEquipStateType::UNEQUIP);
}

void UCharacterAnimInstance_Base::AnimNotify_Collision_Enable()
{
	m_Character->GetWeapon()->SetWeaponCollisionEnable(true);
}

void UCharacterAnimInstance_Base::AnimNotify_Collision_Disable()
{
	m_Character->GetWeapon()->SetWeaponCollisionEnable(false);
}

void UCharacterAnimInstance_Base::AnimNotify_Player_Effect_Start()
{
	m_Character->GetWeapon()->GetNiagara()->Activate();
}

void UCharacterAnimInstance_Base::AnimNotify_Player_Effect_End()
{
	m_Character->GetWeapon()->GetNiagara()->Deactivate();
}
