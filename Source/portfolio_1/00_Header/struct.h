#pragma once

#include "enum.h"
#include "InputAction.h"

#include "Particles\ParticleSystem.h"
#include "NiagaraSystem.h"
#include "Engine/DataTable.h"
#include "Struct.generated.h"

USTRUCT(BlueprintType)
struct FComboInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float	StaminaCost;

	UPROPERTY(EditAnywhere)
	float InvincibleTime;

	UPROPERTY(EditAnywhere)
	FName	SectionName;
};

USTRUCT(BlueprintType)
struct FWeaponAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage>	Montage;

	UPROPERTY(EditAnywhere)
	TArray<FComboInfo> ComboInfo;
};

USTRUCT(BlueprintType)
struct FBossSkillInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage>	Montage;

	UPROPERTY(EditAnywhere)
	float	MinRange;

	UPROPERTY(EditAnywhere)
	float	MaxRange;

	UPROPERTY(EditAnywhere)
	float	MaxCoolTime;

	UPROPERTY(EditAnywhere)
	int32	Probability;

	float	CurCoolTime;
};

USTRUCT(Atomic, BlueprintType)
struct FStatsInfo
	: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString	Name;

	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	HP;

	// 초당 체력 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	HP_Recovery;

	// 마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	MP;
	
	// 초당 마나 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	MP_Recovery;

	// 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	Stamina;

	// 초당 스테미너 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	Stamina_Recovery;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	Att;

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	Def;
};

USTRUCT(Atomic, BlueprintType)
struct FGameItemInfo
	: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EITEM_ID		ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EITEM_TYPE		TYPE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString			Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString			IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Att;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Heal_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Heal_MP;
};

USTRUCT(Atomic, BlueprintType)
struct FEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem>		Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem>		Niagara;
};