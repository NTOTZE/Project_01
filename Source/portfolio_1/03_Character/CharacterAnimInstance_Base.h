// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Character_Base.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance_Base.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UCharacterAnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()

private:

	UPROPERTY(BlueprintReadOnly, Category = "참조", meta = (AllowprivateAccess = "true"))
	ACharacter_Base* m_Character;
	
	UPROPERTY(BlueprintReadOnly, Category = "참조", meta = (AllowprivateAccess = "true"))
	UCharacterMovementComponent* m_Movement;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	float	m_MoveSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	bool	m_IsMove;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	FVector	m_LocalVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	bool	m_IsGround;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	float	m_DistanceTraveled;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	bool	m_HasAccel;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	ECharacterMoveStateType m_MoveAnimState;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	EWeaponEquipStateType m_EquipAnimState;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	float m_MoveAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Info", meta = (AllowprivateAccess = "true"))
	EMOVE_DIRECTION m_MoveDirection;
private:

public:
	//런타임 초기에 한번
	virtual void NativeInitializeAnimation() override;
	//월드 생성될때 호출
	virtual void NativeBeginPlay() override;
	//틱마다 호출
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

private:
	//플레이어 무기 허리춤 Attach 시점 (칼집과 함께 허리춤에 장착된 경우)
	UFUNCTION()
	void AnimNotify_Katana_Attach_1();

	//플레이어 무기 왼손 Attach 시점 (칼집과 함께 왼손에 들려있을 경우)
	UFUNCTION()
	void AnimNotify_Katana_Attach_2();

	//플레이어 무기 오른손 Attach 시점
	UFUNCTION()
	void AnimNotify_Katana_Attach_3();

	//플레이어 무기 칼집 허리춤 Attach 시점
	UFUNCTION()
	void AnimNotify_Scabbard_Attach_1();

	//플레이어 무기 칼집 왼손 Attach 시점
	UFUNCTION()
	void AnimNotify_Scabbard_Attach_2();

	//플레이어 무기 스킬 몽타주 재생 끝 판정 시점
	UFUNCTION()
	void AnimNotify_Mont_End();

	//플레이어 무기 스킬 도중 후속 콤보 혹은 새로운 타입의 공격 시작 가능 시점
	UFUNCTION()
	void AnimNotify_Attackable();

	//플레이어 무기 스킬 콤보 도중 선입력 활성화 시점
	UFUNCTION()
	void AnimNotify_PreInputEnable();

	//플레이어 Equip 몽타주 재생 후 EquipState 변경 시점
	UFUNCTION()
	void AnimNotify_Equip_Weapon();

	//플레이어 Unequip 몽타주 재생 후 EquipState 변경 시점
	UFUNCTION()
	void AnimNotify_Unequip_Weapon();

	//플레이어 무기 충돌 인정 활성화 시점
	UFUNCTION()
	void AnimNotify_Collision_Enable();

	//플레이어 무기 충돌 인정 비활성화 시점
	UFUNCTION()
	void AnimNotify_Collision_Disable();

	//플레이어 무기 나이아가라 이펙트 활성화 시점
	UFUNCTION()
	void AnimNotify_Player_Effect_Start();

	//플레이어 무기 나이아가라 이펙트 비활성화 시점
	UFUNCTION()
	void AnimNotify_Player_Effect_End();
};
