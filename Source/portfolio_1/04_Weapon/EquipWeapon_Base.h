// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "EquipScabbard_Base.h"
#include "../03_Character/Character_Base.h"

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "EquipWeapon_Base.generated.h"

UCLASS()
class PORTFOLIO_1_API AEquipWeapon_Base : public ASkeletalMeshActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowprivateAccess = "true"))
	UCapsuleComponent* m_Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowprivateAccess = "true"))
	UNiagaraComponent* m_Niagara;

	UPROPERTY(EditAnywhere, Category = "Info", meta = (AllowprivateAccess = "true"))
	EWEAPON_TYPE	m_WeaponType;

	UPROPERTY(EditAnywhere, Category = "Info", meta = (AllowprivateAccess = "true"))
	TSoftClassPtr<class AEquipScabbard_Base>	m_ScabbardClass;
	
	UPROPERTY(EditAnywhere, Category = "Info", meta = (AllowprivateAccess = "true"))
	TSoftObjectPtr<class UWeaponComboDataAsset>	m_WCDataAsset;

private:
	TWeakObjectPtr<AEquipScabbard_Base>	m_Scabbard;
	TWeakObjectPtr<ACharacter_Base>		m_Character;

	int32						m_CurComboCount;
	EWeaponAttackType			m_CurAttackType;
	EWeaponAttackType			m_ReservedAttackType;

	bool						m_bAttackable;
	bool						m_bPreInputAble;

	bool						m_bAttackEnhance;

public:
	UCapsuleComponent* GetCapsule() { return m_Capsule; }
	UNiagaraComponent* GetNiagara() { return m_Niagara; }
	AEquipScabbard_Base* GetScabbard() { return m_Scabbard.Get(); }
	ACharacter_Base* GetCharacter() { return m_Character.Get(); }
	void SetWeaponOwner(ACharacter_Base* _Owner) { m_Character = _Owner; }
	EWeaponAttackType GetCurrentAttackType() { return m_CurAttackType; }
	bool IsAttackable() { return m_bAttackable; }
	void PreInputEnable() { m_bPreInputAble = true; }

	void ComboClear();
	void EnableAttack();
	void SetWeaponCollisionEnable(bool _b);

	//UAnimMontage* GetMontage(EInputActionType _Type)
	//{
	//	if (m_Montage[(int32)_Type].IsNull())
	//		return nullptr;
	//	return m_Montage[(int32)_Type].LoadSynchronous();
	//}

private:
	void CreateScabbard();

public:
	void Play_Skill(EWeaponAttackType _AttackType);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	AEquipWeapon_Base();

public:
	void InputAction_L_Shift(const FInputActionInstance& _Instance);
	void InputAction_Dodge(const FInputActionInstance& _Instance);
	void InputAction_Attack_0(const FInputActionInstance& _Instance);
	void InputAction_Attack_1(const FInputActionInstance& _Instance);
	void InputAction_Attack_2(const FInputActionInstance& _Instance);
	void InputAction_Lock_On(const FInputActionInstance& _Instance);
	void InputAction_Tab(const FInputActionInstance& _Instance);

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* _PrimitiveCom
		, AActor* _OtherActor
		, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index
		, bool _bFromSweep
		, const FHitResult& _Hit);
};
