// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipWeapon_Base.h"
#include "NiagaraComponent.h"

#include "../00_System/IADataAsset.h"
#include "../00_System/WeaponComboDataAsset.h"
#include "../02_GameMode/GameMode_Main_Base.h"
#include "../05_Monster/Boss_Base.h"

#include "EnhancedInputComponent.h"

AEquipWeapon_Base::AEquipWeapon_Base()
	: m_WeaponType(EWEAPON_TYPE::NONE)
	, m_CurComboCount(0)
	, m_CurAttackType(EWeaponAttackType::NONE)
	, m_ReservedAttackType(EWeaponAttackType::NONE)
	, m_bAttackable(false)
	, m_bPreInputAble(false)
	, m_bAttackEnhance(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	m_Capsule->SetupAttachment(GetSkeletalMeshComponent());
	m_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	m_Niagara->SetupAttachment(m_Capsule);
	m_Niagara->bAutoActivate = false;
}

void AEquipWeapon_Base::BeginPlay()
{
	Super::BeginPlay();

	if (m_WeaponType == EWEAPON_TYPE::NONE)
	{
		Log(Log_Weapon, Error, TEXT("WeaponType 설정되지 않음"));
	}

	if (m_ScabbardClass.IsNull())
	{
		Log(Log_Weapon, Error, TEXT("Scabbard Class 설정되지 않음"));
	}
	else
	{
		CreateScabbard();
	}

	if (m_WCDataAsset.IsNull())
	{
		Log(Log_Weapon, Error, TEXT("Weapon Combo Data Asset 설정되지 않음"));
	}

	// 웨펀 인풋 세팅
	if (m_Character.IsValid())
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		UEnhancedInputComponent* InputCom = Cast<UEnhancedInputComponent>(m_Character.Get()->InputComponent);
		if (!InputCom)
		{
			Log(Log_Weapon, Error, TEXT("UEnhancedInputComponent 캐스팅 실패"));
			return;
		}
		UIADataAsset* pDataAsset = m_Character.Get()->GetIASetting();
		if (!IsValid(pDataAsset))
		{
			Log(Log_Weapon, Error, TEXT("pCharacter->GetIASetting() 유효하지 않음"));
			return;
		}

		for (int32 i = 0; i < (int32)EWeaponInputActionType::ENUM_END; ++i)
		{
			if (pDataAsset->WIADataArr[i].IsNull())
				continue;

			switch ((EWeaponInputActionType)i)
			{
			case EWeaponInputActionType::L_SHIFT:
				InputCom->BindAction(pDataAsset->WIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this
					, &AEquipWeapon_Base::InputAction_L_Shift);
				break;
			case EWeaponInputActionType::DODGE:
				InputCom->BindAction(pDataAsset->WIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this
					, &AEquipWeapon_Base::InputAction_Dodge);
				break;
			case EWeaponInputActionType::ATTACK_0:
				InputCom->BindAction(pDataAsset->WIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this
					, &AEquipWeapon_Base::InputAction_Attack_0);
				break;
			case EWeaponInputActionType::ATTACK_1:
				InputCom->BindAction(pDataAsset->WIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this
					, &AEquipWeapon_Base::InputAction_Attack_1);
				break;
			case EWeaponInputActionType::ATTACK_2:
				InputCom->BindAction(pDataAsset->WIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this
					, &AEquipWeapon_Base::InputAction_Attack_2);
				break;
			case EWeaponInputActionType::LOCK_ON:
				InputCom->BindAction(pDataAsset->WIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this
					, &AEquipWeapon_Base::InputAction_Lock_On);
				break;
			case EWeaponInputActionType::TAB:
				InputCom->BindAction(pDataAsset->WIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this
					, &AEquipWeapon_Base::InputAction_Tab);
				break;
			default:
				break;
			}
		}

		m_Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEquipWeapon_Base::BeginOverlap);	//오버랩 시작
	}

	m_bAttackable = true;
	m_CurAttackType = EWeaponAttackType::NONE;

	SetWeaponCollisionEnable(false);
}

void AEquipWeapon_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AEquipWeapon_Base::ComboClear()
{
	m_CurComboCount = 0;
	m_bAttackable = true;
	m_bPreInputAble = false;
	m_CurAttackType = EWeaponAttackType::NONE;
	m_ReservedAttackType = EWeaponAttackType::NONE;
	m_Character.Get()->GetWeapon()->SetWeaponCollisionEnable(false);
	//m_Character->SetMoveType(ECharacterMoveStateType::IDLE);
}

void AEquipWeapon_Base::EnableAttack()
{
	m_bPreInputAble = false;
	m_bAttackable = true;
	if (m_ReservedAttackType != EWeaponAttackType::NONE)
		Play_Skill(m_ReservedAttackType);
}

void AEquipWeapon_Base::SetWeaponCollisionEnable(bool _b)
{
	static ECollisionEnabled::Type DefaultCollType = m_Capsule->GetCollisionEnabled();
	if (_b)
	{
		m_Capsule->SetCollisionEnabled(DefaultCollType);
	}
	else
	{
		m_Capsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}

void AEquipWeapon_Base::CreateScabbard()
{
	//스폰할떄 파라메터 넘겨줄수있음
	FActorSpawnParameters param = {};
	//스폰된 액터가 충돌발생시 어떻게 할 것인가 = AlwaysSpawn 그냥 해당 위치에 스폰
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//엑터를 스폰시킬 레벨 설정 = 현재 Character_Base의 레벨
	param.OverrideLevel = GetLevel();
	//지연생성 여부 = true(beginplay가 바로 호출되지않음)
	param.bDeferConstruction = false;

	//FTransform trans = m_Owner->GetMesh()->GetSocketTransform(FName(TEXT("scabbard_1")));
	FTransform trans;

	m_Scabbard = m_Character.Get()->GetWorld()->SpawnActor<AEquipScabbard_Base>(m_ScabbardClass.LoadSynchronous(), trans, param);
	m_Scabbard.Get()->AttachToComponent(m_Character.Get()->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("scabbard_1")));

	if (!m_Scabbard.IsValid())
	{
		Log(Log_Weapon, Error, TEXT("Scabbard 스폰 실패"));
	}
}

void AEquipWeapon_Base::Play_Skill(EWeaponAttackType _AttackType)
{
	if (_AttackType == EWeaponAttackType::NONE || _AttackType == EWeaponAttackType::ENUM_END)
		return;

	if (m_bAttackable == false)
	{
		if (m_bPreInputAble == true)
			m_ReservedAttackType = _AttackType;
		return;
	}

	if (m_CurAttackType != _AttackType)
		ComboClear();

	const FWeaponAttackData& pComboData = m_WCDataAsset.LoadSynchronous()->ComboDataArr[(int32)_AttackType];

	if (m_CurComboCount >= pComboData.ComboInfo.Num())
		return;

	if (pComboData.Montage.IsNull())
	{
		Log(Log_Weapon, Error, TEXT("Attack_0 Montage 세팅되지 않음."));
		return;
	}
	UAnimMontage* pMontage = pComboData.Montage.LoadSynchronous();
	if (!IsValid(pMontage))
	{
		Log(Log_Weapon, Error, TEXT("Attack_0 Montage 유효하지 않음."));
		return;
	}

	if (pComboData.ComboInfo[m_CurComboCount].StaminaCost != 0 &&
		m_Character.Get()->GetCurrentStats().Stamina <= 0.f)
	{
		return;
	}

	m_bAttackable = false;
	m_ReservedAttackType = EWeaponAttackType::NONE;
	m_CurAttackType = _AttackType;

	m_Character.Get()->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);

	m_Character.Get()->GetMesh()->GetAnimInstance()->Montage_Play(pMontage);
	m_Character.Get()->GetMesh()->GetAnimInstance()->Montage_SetPlayRate(pMontage, 1.0f);
	m_Character.Get()->GetMesh()->GetAnimInstance()->Montage_JumpToSection(pComboData.ComboInfo[m_CurComboCount].SectionName, pMontage);
	m_Character.Get()->AddStamina(-pComboData.ComboInfo[m_CurComboCount].StaminaCost);
	m_Character.Get()->SetInvincibleTime(pComboData.ComboInfo[m_CurComboCount].InvincibleTime);

	m_CurComboCount++;
}

void AEquipWeapon_Base::InputAction_L_Shift(const FInputActionInstance& _Instance)
{
	bool bInput = _Instance.GetValue().Get<bool>();
	
	if (bInput)
	{
		if (m_Character.Get()->GetMoveInput().Size() > 0.f)
		{
			m_Character.Get()->SetRunReady(true);
		}
		else
		{
			m_bAttackEnhance = true;
		}
	}
	else
	{
		m_Character.Get()->SetRunReady(false);
		m_Character.Get()->SetSprintReady(false);
		m_bAttackEnhance = false;
	}
}

void AEquipWeapon_Base::InputAction_Dodge(const FInputActionInstance& _Instance)
{
	if (m_Character.Get()->GetMoveType() == ECharacterMoveStateType::IDLE)
	{
		// 백스탭
		if (m_Character.Get()->GetEquipType() == EWeaponEquipStateType::EQUIP)
		{
			if (m_CurAttackType != EWeaponAttackType::BACKSTEP)
			{
				Play_Skill(EWeaponAttackType::BACKSTEP);
			}
		}
	}
	else if (m_Character.Get()->GetMoveType() == ECharacterMoveStateType::WALK)
	{
		//Dodge Montage Play
		if (m_Character.Get()->GetEquipType() == EWeaponEquipStateType::EQUIP)
		{
			FVector2D vec = m_Character.Get()->GetMoveInput();
			if (vec.X > 0.f)
			{
				if (vec.Y > 0.f)
					Play_Skill(EWeaponAttackType::EVADE_FORWARD_RIGHT);
				else if (vec.Y < 0.f)
					Play_Skill(EWeaponAttackType::EVADE_FORWARD_LEFT);
				else
					Play_Skill(EWeaponAttackType::EVADE_FORWARD);
			}
			else if (vec.X < 0.f)
			{
				if (vec.Y > 0.f)
					Play_Skill(EWeaponAttackType::EVADE_BACKWARD_RIGHT);
				else if (vec.Y < 0.f)
					Play_Skill(EWeaponAttackType::EVADE_BACKWARD_LEFT);
				else
					Play_Skill(EWeaponAttackType::EVADE_BACKWARD);
			}
			else
			{
				if (vec.Y > 0.f)
					Play_Skill(EWeaponAttackType::EVADE_RIGHTWARD);
				else if (vec.Y < 0.f)
					Play_Skill(EWeaponAttackType::EVADE_LEFTWARD);
				else
					Play_Skill(EWeaponAttackType::BACKSTEP);
			}
		}
	}
	else if (m_Character.Get()->GetMoveType() == ECharacterMoveStateType::RUN)
	{
		//약간의 대쉬 후 스프린트 진입, 대쉬모션 없으면 걍 생략
		m_Character.Get()->SetSprintReady(true);
	}
}

void AEquipWeapon_Base::InputAction_Attack_0(const FInputActionInstance& _Instance)
{
	if (m_Character.Get()->GetEquipType() == EWeaponEquipStateType::UNEQUIP)
	{
		//Montage Play 납도
		Play_Skill(EWeaponAttackType::EQUIP);
	}
	else if(m_Character.Get()->GetEquipType() == EWeaponEquipStateType::EQUIP)
	{
		if (m_bAttackEnhance)
		{
			Play_Skill(EWeaponAttackType::HEAVY);
		}
		else
		{
			if (m_CurAttackType == EWeaponAttackType::NONE)
			{
				Play_Skill(EWeaponAttackType::NORMAL);
			}
			else
			{
				Play_Skill(m_CurAttackType);
			}
		}
	}
}

void AEquipWeapon_Base::InputAction_Attack_1(const FInputActionInstance& _Instance)
{
	bool bDown = _Instance.GetValue().Get<bool>();

	if (bDown)
	{
		if (m_Character.Get()->GetEquipType() == EWeaponEquipStateType::EQUIP &&
			m_Character.Get()->GetMoveType() != ECharacterMoveStateType::SPRINT)
		{
			m_Character.Get()->SetEquipType(EWeaponEquipStateType::GUARD);
		}
	}
	else
	{
		if (m_Character.Get()->GetEquipType() == EWeaponEquipStateType::GUARD)
		{
			m_Character.Get()->SetEquipType(EWeaponEquipStateType::EQUIP);
		}
	}
}

void AEquipWeapon_Base::InputAction_Attack_2(const FInputActionInstance& _Instance)
{
}

void AEquipWeapon_Base::InputAction_Lock_On(const FInputActionInstance& _Instance)
{
	if (m_Character.Get()->GetTarget() == nullptr)
	{
		//TODO
		// 일단 임시로 게임모드에 고정으로 넣어놓은 보스타겟 받아와서 고정
		// 추후엔 뭐 트레이스를 쓰든가 해서 타게팅 방식 개선필요

		m_Character.Get()->SetTarget(Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()))->m_Boss.Get());
	}
	else
	{
		m_Character.Get()->SetTarget(nullptr);
	}
}

void AEquipWeapon_Base::InputAction_Tab(const FInputActionInstance& _Instance)
{
	if (m_CurAttackType == EWeaponAttackType::NONE)
	{
		if (m_Character.Get()->GetEquipType() == EWeaponEquipStateType::EQUIP)
		{
			Play_Skill(EWeaponAttackType::UNEQUIP);
		}
		else if (m_Character.Get()->GetEquipType() == EWeaponEquipStateType::UNEQUIP)
		{
			Play_Skill(EWeaponAttackType::EQUIP);
		}
	}
}

void AEquipWeapon_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _Hit)
{
}
