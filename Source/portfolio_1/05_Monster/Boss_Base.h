// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "behaviortree/behaviorTree.h"
#include "behaviortree/blackboarddata.h"

#include "Boss_Base.generated.h"

UCLASS()
class PORTFOLIO_1_API ABoss_Base : public ACharacter
{
	GENERATED_BODY()
private:
	// 사용할 행동트리 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowprivateAccess = "true"))
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowprivateAccess = "true"))
	UBlackboardData* m_Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	UCapsuleComponent* m_HitCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	UCapsuleComponent* m_WeaponCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	UCapsuleComponent* m_LeftArmCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	UNiagaraComponent* m_WeaponNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential|Stats", meta = (RowType = "StatsInfo", AllowprivateAccess = "true"))
	FDataTableRowHandle		m_StatsTableRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential|DropItem", meta = (AllowprivateAccess = "true"))
	TSubclassOf<class ADropItem_Base>	m_DropItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential|DropItem", meta = (AllowprivateAccess = "true"))
	EITEM_ID				m_DropItem;

	FStatsInfo				m_DefaultStats;
	FStatsInfo				m_CurStats;

	bool					m_bDead;
	float					m_fBurnIntence;
	const float				Temp_BurnIntenceCoeff = 0.2f;

	TWeakObjectPtr<AActor> m_Target;

	bool	m_bBattleEntrance;
	bool	m_bPlayingMontage;
	bool	m_bEnableLookAt;

	const float Temp_HitDelay_InvincibleTimeTime = 0.3f;

	float m_InvincibleTime;

public:
	// Component
	UBehaviorTree* GetBehaviorTree() { return m_BehaviorTree; }
	UBlackboardData* GetBlackboard() { return m_Blackboard; }
	UCapsuleComponent* GetWeaponCapsule() { return m_WeaponCapsule; }
	UCapsuleComponent* GetLeftArmCapsule() { return m_LeftArmCapsule; }
	UNiagaraComponent* GetNiagara() { return m_WeaponNiagara; }

	// Stats Info
	const FStatsInfo& GetDefaultStats() { return m_DefaultStats; }
	const FStatsInfo& GetCurrentStats() { return m_CurStats; }

	// Battle Info
	AActor* GetTarget() { return m_Target.Get(); }
	void SetTarget(AActor* _Target) { m_Target = _Target; }

	bool GetBattleEntrance() { return m_bBattleEntrance; }
	void SetBattleEntrance(bool _b) { m_bBattleEntrance = _b; }

	bool GetPlayingMontage() { return m_bPlayingMontage; }
	void SetPlayingMontage(bool _b) { m_bPlayingMontage = _b; }

	void EnableLookAtTarget(bool _b) { m_bEnableLookAt = _b; }

	float GetInvincibleTime() { return m_InvincibleTime; }
	void SetInvincibleTime(float _Time) { m_InvincibleTime = _Time; }

public:
	void BattleEntrance();
	void Died();
	bool OnAttacked(float _Damage);
	void SetWeaponCollisionEnable(bool _b);
	void SetLeftArmCollisionEnable(bool _b);
	void CreateDropItem();

public:
	// Sets default values for this character's properties
	ABoss_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	UFUNCTION()
	void BodyBeginOverlap(UPrimitiveComponent* _PrimitiveCom
		, AActor* _OtherActor
		, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index
		, bool _bFromSweep
		, const FHitResult& _Hit);
};
