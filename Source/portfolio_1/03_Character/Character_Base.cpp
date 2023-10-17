// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "../04_Weapon/EquipWeapon_Base.h"
#include "../00_System/IADataAsset.h"
#include "../02_GameMode/GameMode_Main_Base.h"
#include "../05_Monster/Boss_Base.h"
#include "../01_Manager/EffectMgr.h"
#include "../01_Manager/InvenMgr.h"
#include "../09_DropItem/DropItem_Base.h"
#include "../08_UI/MainHUD_Base.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
	: m_IsGround(true)
	, m_DistanceTraveled(0.f)
	, m_bRun(false)
	, m_bSprint(false)
	, m_MoveAngle(0.f)
	, m_MoveDir(EMOVE_DIRECTION::FORWARD)
	, m_MoveType(ECharacterMoveStateType::IDLE)
	, m_EquipType(EWeaponEquipStateType::NOWEAPON)
	, m_Weapon(nullptr)
	, m_Target(nullptr)
	, m_InvincibleTime(0.f)
	, m_bDead(false)
	, m_bInteractable(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCapsule"));
	m_InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	m_HitCapsule->SetupAttachment(GetCapsuleComponent());
	m_InteractionCapsule->SetupAttachment(GetCapsuleComponent());
	m_Arm->SetupAttachment(GetCapsuleComponent());
	m_Cam->SetupAttachment(m_Arm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	GetCapsuleComponent()->SetCapsuleRadius(40.f);

	m_InteractionCapsule->SetCapsuleHalfHeight(90.f);
	m_InteractionCapsule->SetCapsuleRadius(40.f);
	m_InteractionCapsule->SetRelativeLocation(FVector(30.f, 0.f, 0.f));
	
	m_HitCapsule->SetCapsuleHalfHeight(90.f);
	m_HitCapsule->SetCapsuleRadius(25.f);

	m_Arm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	m_Arm->TargetArmLength = 300.f;
	m_Arm->bInheritYaw = false;
	m_Arm->bInheritPitch = false;
	m_Arm->bInheritRoll = false;

	m_Arm->bEnableCameraLag = true;
	m_Arm->CameraLagSpeed = 3.f;
	m_Arm->bEnableCameraRotationLag = true;
	m_Arm->CameraRotationLagSpeed = 10.f;

	m_Cam->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f ,540.f, 0.f);
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()))->m_Player = this;

	APlayerController* pController = Cast<APlayerController>(GetController());
	if (pController)
	{
		ULocalPlayer* pLocalPlayer = pController->GetLocalPlayer();
		if (pLocalPlayer && !InputMapping.IsNull())
		{
			UEnhancedInputLocalPlayerSubsystem* pSubsystem = pLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			pSubsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
		}
	}
	m_CurActorPos = GetActorLocation();

	if (m_WeaponClass.IsNull())
	{
		Log(Log_Player, Error, TEXT("Weapon Class 설정되지 않음"));
	}
	else
	{
		CreateWeapon();
		SetEquipType(EWeaponEquipStateType::UNEQUIP);
	}

	if (IsValid(m_StatsTableRow.DataTable) && !m_StatsTableRow.RowName.IsNone())
	{
		FStatsInfo* pInfo = m_StatsTableRow.DataTable->FindRow<FStatsInfo>(m_StatsTableRow.RowName, TEXT(""));
		if (pInfo)
		{
			m_DefaultStats = *pInfo;
			m_CurStats = m_DefaultStats;
		}
		else
		{
			Log(Log_Player, Error, TEXT("Stats Data Table -> FindRow = nullptr"));
		}
	}
	else
	{
		Log(Log_Player, Error, TEXT("Stats Data Table 설정되지 않음"));
	}

	FRotator rot = m_Arm->GetRelativeRotation();
	rot.Yaw = GetActorRotation().Yaw;
	m_Arm->SetRelativeRotation(rot);

	m_HitCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Base::Hit_BeginOverlap);
	m_InteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Base::Interaction_BeginOverlap);
	m_InteractionCapsule->OnComponentEndOverlap.AddDynamic(this, &ACharacter_Base::Interaction_EndOverlap);

}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 무적 시간 카운트
	if (m_InvincibleTime < DeltaTime)
		m_InvincibleTime = 0.f;
	else
		m_InvincibleTime -= DeltaTime;

	// 락온 카메라 설정
	if (m_Target.IsValid())
	{
		m_Arm->SetRelativeRotation((m_Target.Get()->GetActorLocation() - GetActorLocation()).Rotation());
	}

	// 캐릭터 시선 카메라 추적
	if (GetCharacterMovement()->bOrientRotationToMovement == false)
	{
		FRotator ActRot = GetActorRotation();
		float fCameraYaw = m_Arm->GetRelativeRotation().Yaw;
		float fYawDiff = fCameraYaw - ActRot.Yaw;
		const float fMaxRotYaw = GetCharacterMovement()->RotationRate.Yaw * DeltaTime;

		if (fYawDiff > 180.f)
			fYawDiff -= 360.f;
		else if (fYawDiff < -180.f)
			fYawDiff += 360.f;

		if (fYawDiff > fMaxRotYaw)
			ActRot.Yaw += fMaxRotYaw;
		else if(fYawDiff < -fMaxRotYaw)
			ActRot.Yaw -= fMaxRotYaw;
		else
			ActRot.Yaw = fCameraYaw;

		SetActorRotation(ActRot);
	}
	
	m_LocalVelocity = GetActorRotation().UnrotateVector(GetCharacterMovement()->Velocity);

	//이동 방향 계산
	m_MoveAngle = m_LocalVelocity.Rotation().Yaw;
	
	if (m_MoveAngle > 157.5f)
		m_MoveDir = EMOVE_DIRECTION::BACKWARD;
	else if (m_MoveAngle > 112.5f)
		m_MoveDir = EMOVE_DIRECTION::BACKWARD_RIGHT;
	else if (m_MoveAngle > 67.5f)
		m_MoveDir = EMOVE_DIRECTION::RIGHTWARD;
	else if (m_MoveAngle > 22.5f)
		m_MoveDir = EMOVE_DIRECTION::FORWARD_RIGHT;
	else if (m_MoveAngle > -22.5f)
		m_MoveDir = EMOVE_DIRECTION::FORWARD;
	else if (m_MoveAngle > -67.5f)
		m_MoveDir = EMOVE_DIRECTION::FORWARD_LEFT;
	else if (m_MoveAngle > -112.5f)
		m_MoveDir = EMOVE_DIRECTION::LEFTWARD;
	else if (m_MoveAngle > -157.5f)
		m_MoveDir = EMOVE_DIRECTION::BACKWARD_LEFT;
	else
		m_MoveDir = EMOVE_DIRECTION::BACKWARD;



	m_IsGround = !GetCharacterMovement()->IsFalling();

	FVector vecCurLocation = GetActorLocation();
	m_DistanceTraveled = FVector::Dist2D(m_CurActorPos, vecCurLocation);
	m_CurActorPos = vecCurLocation;

/////////////////////////////////////
// Move Type 세팅
	if (m_LocalVelocity.Size2D() <= 0.f)
	{
		SetMoveType(ECharacterMoveStateType::IDLE);
	}
	else if (m_MoveInput.Size() <= 0.f)
	{
		SetMoveType(ECharacterMoveStateType::IDLE);
	}
	else
	{
		if (GetMoveType() == ECharacterMoveStateType::SPRINT)
		{
			if (!m_bRun)
				SetMoveType(ECharacterMoveStateType::WALK);
		}
		else if (GetMoveType() == ECharacterMoveStateType::RUN)
		{
			if (!m_bRun || GetEquipType() == EWeaponEquipStateType::GUARD)
				SetMoveType(ECharacterMoveStateType::WALK);
			else if (m_bSprint)
				SetMoveType(ECharacterMoveStateType::SPRINT);
		}
		else if(GetMoveType() == ECharacterMoveStateType::WALK)
		{
			if (m_bRun)
				SetMoveType(ECharacterMoveStateType::RUN);
		}
		else if (GetMoveType() == ECharacterMoveStateType::IDLE)
		{
			if (m_bRun)
				SetMoveType(ECharacterMoveStateType::RUN);
			else
				SetMoveType(ECharacterMoveStateType::WALK);
		}
	}

/////////////////////////////////////
// 캐릭터 방향 세팅 (카메라방향 or 무브먼트 방향)
	if (GetEquipType() == EWeaponEquipStateType::GUARD)
	{
		EnableFreeCamera(false);
	}
	else if (GetEquipType() == EWeaponEquipStateType::EQUIP)
	{
		if(GetMoveType() == ECharacterMoveStateType::SPRINT)
			EnableFreeCamera(true);
		else
			EnableFreeCamera(false);
	}
	else if (GetEquipType() == EWeaponEquipStateType::UNEQUIP)
	{
		EnableFreeCamera(true);
	}
	else if (GetEquipType() == EWeaponEquipStateType::NOWEAPON)
	{
		EnableFreeCamera(true);
	}
	else
	{
		EnableFreeCamera(true);
	}

///////////////////////////////////////
// 스태미너
	if (m_EquipType == EWeaponEquipStateType::UNEQUIP ||
		m_EquipType == EWeaponEquipStateType::NOWEAPON)
	{
		m_CurStats.Stamina = m_DefaultStats.Stamina;
	}
	else if (m_EquipType == EWeaponEquipStateType::EQUIP)
	{
		if (m_MoveType == ECharacterMoveStateType::IDLE || 
			m_MoveType == ECharacterMoveStateType::WALK)
		{
			if (m_Weapon->GetCurrentAttackType() == EWeaponAttackType::NONE)
			{
				AddStamina(DeltaTime * m_CurStats.Stamina_Recovery);
			}
		}
		else if (m_MoveType == ECharacterMoveStateType::SPRINT)
		{
			AddStamina(-DeltaTime * m_CurStats.Stamina_Recovery);
		}
	}
	else if (m_EquipType == EWeaponEquipStateType::GUARD)
	{

	}
}

// Called to bind functionality to input
void ACharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputCom = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (nullptr == InputCom)
		return;

	if (!InputActionSetting.IsNull())
	{
		UIADataAsset* pDataAsset = InputActionSetting.LoadSynchronous();

		for (int32 i = 0; i < (int32)ECharacterInputActionType::ENUM_END; ++i)
		{
			if (pDataAsset->CIADataArr[i].IsNull())
				continue;

			//switch (pDataAsset->CIADataArr[i].Type)
			switch ((ECharacterInputActionType)i)
			{
			case ECharacterInputActionType::MOVE_FRONT:
				InputCom->BindAction(pDataAsset->CIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this,
					&ACharacter_Base::InputAction_Move_Front);
				break;
			case ECharacterInputActionType::MOVE_RIGHT:
				InputCom->BindAction(pDataAsset->CIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this,
					&ACharacter_Base::InputAction_Move_Right);
				break;
			case ECharacterInputActionType::ROTATION:
				InputCom->BindAction(pDataAsset->CIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this,
					&ACharacter_Base::InputAction_Rotation);
				break;
			case ECharacterInputActionType::INTERACTION:
				InputCom->BindAction(pDataAsset->CIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this,
					&ACharacter_Base::InputAction_Interaction);
				break;
			case ECharacterInputActionType::INVENTORY:
				InputCom->BindAction(pDataAsset->CIADataArr[i].LoadSynchronous(), ETriggerEvent::Triggered, this,
					&ACharacter_Base::InputAction_Inventory);
				break;
			default:
				break;
			}
		}
	}
}

void ACharacter_Base::InputAction_Move_Front(const FInputActionInstance& _Instance)
{
	//Log(Log_Player, Warning, TEXT(""));


	FVector2d vec = _Instance.GetValue().Get<FVector2D>();
	m_MoveInput.X = vec.X - vec.Y;
	FVector vecMove = FRotator(0.f, m_Arm->GetRelativeRotation().Yaw, 0.f).RotateVector(FVector(1.f, 0.f, 0.f)) * m_MoveInput.X;
	GetCharacterMovement()->AddInputVector(vecMove);
}

void ACharacter_Base::InputAction_Move_Right(const FInputActionInstance& _Instance)
{
	//Log(Log_Player, Warning, TEXT(""));
	FVector2d vec = _Instance.GetValue().Get<FVector2D>();
	m_MoveInput.Y = vec.X - vec.Y;
	FVector vecMove = FRotator(0.f, m_Arm->GetRelativeRotation().Yaw, 0.f).RotateVector(FVector(0.f, 1.f, 0.f)) * m_MoveInput.Y;
	GetCharacterMovement()->AddInputVector(vecMove);
}

void ACharacter_Base::InputAction_Rotation(const FInputActionInstance& _Instance)
{
	//Log(Log_Player, Warning, TEXT(""));
	if (m_Target.IsValid())
		return;

	FVector2D vec = _Instance.GetValue().Get<FVector2D>();

	FRotator rot = m_Arm->GetRelativeRotation();
	float fDT = GetWorld()->GetDeltaSeconds();
	rot.Yaw += vec.X * fDT * 100.f;
	rot.Pitch += vec.Y * fDT * 100.f;

	if (rot.Pitch > 80.f)
		rot.Pitch = 80.f;
	else if (rot.Pitch < -80.f)
		rot.Pitch = -80.f;

	m_Arm->SetRelativeRotation(rot);
}

void ACharacter_Base::InputAction_Interaction(const FInputActionInstance& _Instance)
{
	Log(Log_Player, Warning, TEXT(""));
	if (IsInteractable())
	{
		TArray<FHitResult> HitRes;
		const bool bCollision = Interaction_Check(HitRes);
		if (bCollision)
		{
			for (int32 i = 0; i < HitRes.Num(); ++i)
			{
				ADropItem_Base* pItem = Cast<ADropItem_Base>(HitRes[i].GetActor());
				if (pItem)
				{
					EITEM_ID PickItem = pItem->PickItem();
					GetGameInstance()->GetSubsystem<UInvenMgr>()->AddGameItem(PickItem);
					UE_LOG(LogTemp, Warning, TEXT("%d"), (int32)PickItem);
					break;
				}
			}
			EnableInteraction(Interaction_Check(HitRes));
		}
	}
}

void ACharacter_Base::InputAction_Inventory(const FInputActionInstance& _Instance)
{
	Log(Log_Player, Warning, TEXT(""));
	GetGameInstance()->GetSubsystem<UInvenMgr>()->OpenInventoryUI();
}

void ACharacter_Base::Hit_BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _Hit)
{
	if (IsDead())
		return;

	if (GetInvincibleTime() > 0.f)
		return;
	SetInvincibleTime(Temp_HitDelay_InvincibleTimeTime);

	TArray<FHitResult> ResultArray;
	FCollisionQueryParams query(NAME_None, false, this);
	
	UCapsuleComponent* pCapsule = Cast<UCapsuleComponent>(_PrimitiveCom);
	float fCapsuleHalfHeight = pCapsule->GetScaledCapsuleHalfHeight();
	float fCapsuleRadius = pCapsule->GetScaledCapsuleRadius();

	bool bCollision = GetWorld()->SweepMultiByChannel(ResultArray
		, pCapsule->GetComponentLocation() + pCapsule->GetComponentRotation().RotateVector(FVector(0.f, 0.f, -(fCapsuleHalfHeight - fCapsuleRadius)))
		, pCapsule->GetComponentLocation() + pCapsule->GetComponentRotation().RotateVector(FVector(0.f, 0.f, (fCapsuleHalfHeight - fCapsuleRadius)))
		, FQuat::Identity
		, Hit_TraceChannel
		, FCollisionShape::MakeSphere(fCapsuleRadius)
		, query);

	if (bCollision)
	{
		for (int32 i = 0; i < ResultArray.Num(); ++i)
		{
			ABoss_Base* pBoss = Cast<ABoss_Base>(ResultArray[i].GetActor());
			if (pBoss)
			{
				if (pBoss->GetWeaponCapsule() == ResultArray[i].GetComponent())
				{
					bool bSuccess = OnAttacked(pBoss->GetCurrentStats().Att);

					FTransform trans;
					trans.SetLocation(ResultArray[i].ImpactPoint);
					
					if (bSuccess)
					{
						GetGameInstance()->GetSubsystem<UEffectMgr>()->CreateEffect(GetWorld(), EEFFECT_TYPE::BOSS_ATTACK_HIT,
							GetLevel(), trans);
					}
					else
					{
						GetGameInstance()->GetSubsystem<UEffectMgr>()->CreateEffect(GetWorld(), EEFFECT_TYPE::BOSS_ATTACK_GUARD,
							GetLevel(), trans);
					}
				}
				else if (pBoss->GetLeftArmCapsule() == ResultArray[i].GetComponent())
				{
					bool bSuccess = OnAttacked(pBoss->GetCurrentStats().Att);

					FTransform trans;
					trans.SetLocation(ResultArray[i].ImpactPoint);

					if (bSuccess)
					{
						GetGameInstance()->GetSubsystem<UEffectMgr>()->CreateEffect(GetWorld(), EEFFECT_TYPE::BOSS_ATTACK_HIT,
							GetLevel(), trans);
					}
					else
					{
						GetGameInstance()->GetSubsystem<UEffectMgr>()->CreateEffect(GetWorld(), EEFFECT_TYPE::BOSS_ATTACK_GUARD,
							GetLevel(), trans);
					}
				}
			}
		}
	}
}

void ACharacter_Base::Interaction_BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _Hit)
{
	if (IsDead())
		return;
	TArray<FHitResult> HitRes;
	const bool bInter = Interaction_Check(HitRes);
	EnableInteraction(bInter);
}

void ACharacter_Base::Interaction_EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	if (IsDead())
		return;
	TArray<FHitResult> HitRes;
	Interaction_Check(HitRes);
	const bool bInter = Interaction_Check(HitRes);
	EnableInteraction(bInter);
}

void ACharacter_Base::AddStamina(float _Cost)
{
	const float fStamina = m_CurStats.Stamina + _Cost;

	if (fStamina > m_DefaultStats.Stamina)
		m_CurStats.Stamina = m_DefaultStats.Stamina;
	else if (fStamina < 0.f)
		m_CurStats.Stamina = 0.f;
	else
		m_CurStats.Stamina = fStamina;
}

void ACharacter_Base::EnableFreeCamera(bool _b)
{
	GetCharacterMovement()->bOrientRotationToMovement = _b;
}

void ACharacter_Base::CreateWeapon()
{
	//스폰할떄 파라메터 넘겨줄수있음
	FActorSpawnParameters param = {};
	//스폰된 액터가 충돌발생시 어떻게 할 것인가 = AlwaysSpawn 그냥 해당 위치에 스폰
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//엑터를 스폰시킬 레벨 설정 = 현재 Character_Base의 레벨
	param.OverrideLevel = GetLevel();
	//지연생성 여부 = true(beginplay가 바로 호출되지않음)
	param.bDeferConstruction = true;

	//FTransform trans = GetMesh()->GetSocketTransform(FName(TEXT("katana_1")));
	FTransform trans;

	m_Weapon = GetWorld()->SpawnActor<AEquipWeapon_Base>(m_WeaponClass.LoadSynchronous(), trans, param);
	m_Weapon->SetWeaponOwner(this);
	m_Weapon->FinishSpawning(m_Weapon->GetTransform());
	m_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("katana_1")));

	if (!IsValid(m_Weapon))
	{
		Log(Log_Weapon, Error, TEXT("Weapon 스폰 실패"));
	}
}

bool ACharacter_Base::OnAttacked(float _Damage)
{
	Log(Log_Player, Warning, TEXT(""));

	if (m_EquipType == EWeaponEquipStateType::GUARD &&
		m_Weapon->IsAttackable())
	{
		if (m_CurStats.Stamina > _Damage)
		{
			AddStamina(-_Damage);
			m_Weapon->ComboClear();
			m_Weapon->Play_Skill(EWeaponAttackType::GUARD_HIT_3);
			return false;
		}
		else
		{
			m_CurStats.Stamina = 0.f;
			m_Weapon->ComboClear();
			m_Weapon->Play_Skill(EWeaponAttackType::GUARD_BREAK);
			return false;
		}
	}
	else
	{
		if (m_CurStats.HP > _Damage)
		{
			m_CurStats.HP -= _Damage;
			m_Weapon->ComboClear();
			m_Weapon->Play_Skill(EWeaponAttackType::HIT_3);
			return true;
		}
		else
		{
			m_CurStats.HP = 0.f;
			// 사망 애니메이션
			return true;
		}
	}
	return false;
}

bool ACharacter_Base::Interaction_Check(TArray<FHitResult> &_ResultArray)
{
	FCollisionQueryParams query(NAME_None, false, this);

	const float fCapsuleHalfHeight = m_InteractionCapsule->GetScaledCapsuleHalfHeight();
	const float fCapsuleRadius = m_InteractionCapsule->GetScaledCapsuleRadius();

	const bool bCollision = GetWorld()->SweepMultiByChannel(_ResultArray
		, m_InteractionCapsule->GetComponentLocation() + m_InteractionCapsule->GetComponentRotation().RotateVector(FVector(0.f, 0.f, -(fCapsuleHalfHeight - fCapsuleRadius)))
		, m_InteractionCapsule->GetComponentLocation() + m_InteractionCapsule->GetComponentRotation().RotateVector(FVector(0.f, 0.f, (fCapsuleHalfHeight - fCapsuleRadius)))
		, FQuat::Identity
		, Interaction_TraceChannel
		, FCollisionShape::MakeSphere(fCapsuleRadius)
		, query);

	return bCollision;
}

void ACharacter_Base::EnableInteraction(bool _b)
{
	AGameMode_Main_Base* pGamemode = Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()));
	if (pGamemode)
	{
		if (_b)
		{
			m_bInteractable = true;
			pGamemode->GetMainHUD()->ShowItemInteractionWidget(true);
		}
		else
		{
			m_bInteractable = false;
			pGamemode->GetMainHUD()->ShowItemInteractionWidget(false);
		}
	}
}
