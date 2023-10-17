// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Base.h"

#include "NiagaraComponent.h"

#include "00_AI/AIController_Boss.h"
#include "../02_GameMode/GameMode_Main_Base.h"
#include "BossAnimInstance_Base.h"
#include "../03_Character/Character_Base.h"
#include "../04_Weapon/EquipWeapon_Base.h"
#include "../01_Manager/EffectMgr.h"
#include "../09_DropItem/DropItem_Base.h"
#include "../08_UI/MainHUD_Base.h"
#include "../08_UI/BossInfo_Base.h"



// Sets default values
ABoss_Base::ABoss_Base()
	: m_DropItem(EITEM_ID::NONE)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIControllerClass = AAIController_Boss::StaticClass();

	m_WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCapsule"));
	m_LeftArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArmCapsule"));
	m_HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCapsule"));
	m_WeaponNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponNiagara"));
	
	ConstructorHelpers::FClassFinder<ADropItem_Base> DropItemFinder(TEXT("/Script/Engine.Blueprint'/Game/00_BlueprintClass/DropItem/BPC_DropItem.BPC_DropItem_C'"));
	if (DropItemFinder.Succeeded())
	{
		m_DropItemClass = DropItemFinder.Class;
	}

	m_WeaponCapsule->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("Weapon_Socket")));
	m_LeftArmCapsule->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("lowerarm_l_Socket")));
	m_HitCapsule->SetupAttachment(GetCapsuleComponent());
	m_WeaponNiagara->SetupAttachment(m_WeaponCapsule);

	m_WeaponNiagara->bAutoActivate = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

// Called when the game starts or when spawned
void ABoss_Base::BeginPlay()
{
	Super::BeginPlay();

	Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()))->m_Boss = this;

	// 스텟
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
			Log(Log_Monster, Error, TEXT("Stats Data Table -> FindRow = nullptr"));
		}
	}
	else
	{
		Log(Log_Monster, Error, TEXT("Stats Data Table 설정되지 않음"));
	}

	SetWeaponCollisionEnable(false);
	SetLeftArmCollisionEnable(false);
	m_fBurnIntence = 0.f;

	m_HitCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Base::BodyBeginOverlap);	//오버랩 시작
}

// Called every frame
void ABoss_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bDead)
	{
		m_fBurnIntence += DeltaTime * Temp_BurnIntenceCoeff;
		if (m_fBurnIntence > 1.f)
			m_fBurnIntence = 1.f;

		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Burn Intence"), m_fBurnIntence);
		return;
	}

	// 무적 시간 카운트
	if (m_InvincibleTime < DeltaTime)
		m_InvincibleTime = 0.f;
	else
		m_InvincibleTime -= DeltaTime;


	if (m_bEnableLookAt)
	{
		FRotator ActRot = GetActorRotation();
		float fTargetYaw = (m_Target.Get()->GetActorLocation() - GetActorLocation()).Rotation().Yaw;
		float fYawDiff = fTargetYaw - ActRot.Yaw;
		const float fMaxRotYaw = GetCharacterMovement()->RotationRate.Yaw * DeltaTime;

		if (fYawDiff > 180.f)
			fYawDiff -= 360.f;
		else if (fYawDiff < -180.f)
			fYawDiff += 360.f;

		if (fYawDiff > fMaxRotYaw)
			ActRot.Yaw += fMaxRotYaw;
		else if (fYawDiff < -fMaxRotYaw)
			ActRot.Yaw -= fMaxRotYaw;
		else
			ActRot.Yaw = fTargetYaw;

		SetActorRotation(ActRot);

		//float fDist = FVector::Distance(GetTarget()->GetActorLocation(), GetActorLocation());
		//UE_LOG(LogTemp, Warning, TEXT("Dist : %f"), fDist);
	}
}

// Called to bind functionality to input
void ABoss_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss_Base::BodyBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _Hit)
{
	if (!GetBattleEntrance())
		return;
		
	if (m_bDead)
		return;

	if (GetCurrentStats().HP <= 0.f)
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
		, ECC_GameTraceChannel7
		, FCollisionShape::MakeSphere(fCapsuleRadius)
		, query);

	if (bCollision)
	{
		for (int32 i = 0; i < ResultArray.Num(); ++i)
		{
			AEquipWeapon_Base* pCharacterWeapon = Cast<AEquipWeapon_Base>(ResultArray[i].GetActor());
			if (pCharacterWeapon)
			{
				if (pCharacterWeapon->GetCapsule() == ResultArray[i].GetComponent())
				{
					bool bSuccess = OnAttacked(pCharacterWeapon->GetCharacter()->GetCurrentStats().Att);

					FTransform trans;
					trans.SetLocation(ResultArray[i].ImpactPoint);

					if (bSuccess)
					{
						GetGameInstance()->GetSubsystem<UEffectMgr>()->CreateEffect(GetWorld(), EEFFECT_TYPE::PLAYER_ATTACK_HIT,
							GetLevel(), trans);
					}
					else
					{
					}
				}
			}
		}
	}
}

void ABoss_Base::BattleEntrance()
{
	SetBattleEntrance(true);
	AGameMode_Main_Base* pGameMode = Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()));
	if (pGameMode)
	{
		pGameMode->GetMainHUD()->GetBossInfoWidget()->SetTextBlock(GetCurrentStats().Name);
		pGameMode->GetMainHUD()->ShowBossInfoWidget(true);
	}
}

void ABoss_Base::Died()
{
	m_bDead = true;
	CreateDropItem();
	SetLifeSpan(1.f / Temp_BurnIntenceCoeff);
	
	AGameMode_Main_Base* pGameMode = Cast<AGameMode_Main_Base>(UGameplayStatics::GetGameMode(GetWorld()));
	if (pGameMode)
	{
		pGameMode->GetMainHUD()->ShowBossInfoWidget(false);
	}
}

bool ABoss_Base::OnAttacked(float _Damage)
{
	Log(Log_Monster, Warning, TEXT(""));
	if (m_CurStats.HP < _Damage)
	{
		m_CurStats.HP = 0.f;
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
		SetPlayingMontage(false);
	}
	else
	{
		m_CurStats.HP -= _Damage;
	}
	return true;
}

void ABoss_Base::SetWeaponCollisionEnable(bool _b)
{
	static ECollisionEnabled::Type DefaultCollType = m_WeaponCapsule->GetCollisionEnabled();
	if (_b)
	{
		m_WeaponCapsule->SetCollisionEnabled(DefaultCollType);
	}
	else
	{
		m_WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}

void ABoss_Base::SetLeftArmCollisionEnable(bool _b)
{
	static ECollisionEnabled::Type DefaultCollType = m_LeftArmCapsule->GetCollisionEnabled();
	if (_b)
	{
		m_LeftArmCapsule->SetCollisionEnabled(DefaultCollType);
	}
	else
	{
		m_LeftArmCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}

void ABoss_Base::CreateDropItem()
{	
	if (m_DropItem == EITEM_ID::NONE || m_DropItem == EITEM_ID::ENUM_END)
	{
		Log(Log_Monster, Error, TEXT("ITEM_ID 설정되지 않음"));
		return;
	}

	if (!IsValid(m_DropItemClass))
	{
		Log(Log_Monster, Error, TEXT("DropItemClass 유효하지 않음"));
		return;
	}

	FActorSpawnParameters param = {};
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	param.OverrideLevel = GetLevel();
	param.bDeferConstruction = true;

	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("spine_03"))) + GetActorForwardVector() * 35.f;

	FTransform trans;
	trans.SetLocation(SpawnLocation);
	trans.SetRotation(GetActorRotation().Quaternion());

	ADropItem_Base* pDropItem = GetWorld()->SpawnActor<ADropItem_Base>(m_DropItemClass, trans, param);

	pDropItem->SetItemID(m_DropItem);
	pDropItem->FinishSpawning(trans);
}
