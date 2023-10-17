// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"
#include "../00_System/IADataAsset.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputMappingContext.h"

#include "Character_Base.generated.h"


UCLASS()
class PORTFOLIO_1_API ACharacter_Base : public ACharacter
{
	GENERATED_BODY()

private:
	const ECollisionChannel Interaction_TraceChannel = ECC_GameTraceChannel10;
	const ECollisionChannel Hit_TraceChannel = ECC_GameTraceChannel6;

	const float Temp_WalkSpeed = 200.f;
	const float Temp_RunSpeed = 375.f;
	const float Temp_SprintSpeed = 460.f;

	const float Temp_HitDelay_InvincibleTimeTime = 0.3f;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowprivateAccess = "true"))
	UCapsuleComponent*						m_HitCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowprivateAccess = "true"))
	UCapsuleComponent*						m_InteractionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowprivateAccess = "true"))
	USpringArmComponent*					m_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowprivateAccess = "true"))
	UCameraComponent*						m_Cam;

	UPROPERTY(EditAnywhere, Category = "Essential|Weapon", meta = (AllowprivateAccess = "true"))
	TSoftClassPtr<class AEquipWeapon_Base>	m_WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Essential|Input", meta = (AllowprivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext>	InputMapping;

	UPROPERTY(EditAnywhere, Category = "Essential|Input", meta = (AllowprivateAccess = "true"))
	TSoftObjectPtr<UIADataAsset>			InputActionSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essential|Stats", meta = (RowType = "StatsInfo", AllowprivateAccess = "true"))
	FDataTableRowHandle						m_StatsTableRow;

private:
	FStatsInfo					m_DefaultStats;
	FStatsInfo					m_CurStats;

	FVector2D					m_MoveInput;	//Acceleration
	FVector						m_LocalVelocity;		//Velocity
	bool						m_IsGround;
	float						m_DistanceTraveled;
	bool						m_bRun;
	bool						m_bSprint;
	float						m_MoveAngle;
	EMOVE_DIRECTION				m_MoveDir;

	ECharacterMoveStateType		m_MoveType;
	EWeaponEquipStateType		m_EquipType;

	UPROPERTY()
	class AEquipWeapon_Base*	m_Weapon;
	TWeakObjectPtr<AActor>		m_Target;

	float						m_InvincibleTime;
	bool						m_bDead;
	bool						m_bInteractable;

public:
	FVector2D GetMoveInput() { return m_MoveInput; }
	FVector GetLocalVelocity() { return m_LocalVelocity; }
	bool IsGround() { return m_IsGround; }
	float GetDistanceTraveled() { return m_DistanceTraveled; }
	UIADataAsset* GetIASetting() { return InputActionSetting.LoadSynchronous(); }

	// Stats Info
	const FStatsInfo& GetDefaultStats() { return m_DefaultStats; }
	const FStatsInfo& GetCurrentStats() { return m_CurStats; }

	void SetRunReady(bool _b) { m_bRun = _b; }
	void SetSprintReady(bool _b) { m_bSprint = _b; }
	float GetMoveAngle() { return m_MoveAngle; }
	EMOVE_DIRECTION GetMoveDirection() { return m_MoveDir; }

	ECharacterMoveStateType GetMoveType() { return m_MoveType; }
	EWeaponEquipStateType GetEquipType() { return m_EquipType; }

	AEquipWeapon_Base* GetWeapon() { return m_Weapon; }
	void SetTarget(AActor* _Target) { m_Target = _Target; }
	AActor* GetTarget() { return m_Target.Get(); }
	void SetInvincibleTime(float _Time) { m_InvincibleTime = _Time; }
	float GetInvincibleTime() { return m_InvincibleTime; }
	bool IsDead() { return m_bDead; }
	bool IsInteractable() { return m_bInteractable; }

	void AddStamina(float _Cost);

	void SetMoveType(ECharacterMoveStateType _Type)
	{
		m_MoveType = _Type;
		m_bSprint = false;
		switch (m_MoveType)
		{
		case ECharacterMoveStateType::IDLE:
			GetCharacterMovement()->MaxWalkSpeed = Temp_WalkSpeed;
			break;
		case ECharacterMoveStateType::WALK:
			GetCharacterMovement()->MaxWalkSpeed = Temp_WalkSpeed;
			break;
		case ECharacterMoveStateType::RUN:
			GetCharacterMovement()->MaxWalkSpeed = Temp_RunSpeed;
			break;
		case ECharacterMoveStateType::SPRINT:
			GetCharacterMovement()->MaxWalkSpeed = Temp_SprintSpeed;
			break;
		default:
			break;
		}
	}

	void SetEquipType(EWeaponEquipStateType _Type)
	{
		m_EquipType = _Type;
		m_bSprint = false;
		switch (m_EquipType)
		{
		case EWeaponEquipStateType::NOWEAPON:
			break;
		case EWeaponEquipStateType::UNEQUIP:
			break;
		case EWeaponEquipStateType::EQUIP:
			break;
		case EWeaponEquipStateType::GUARD:
			break;
		case EWeaponEquipStateType::ENUM_END:
			break;
		default:
			break;
		}
	}

private:
	FVector			m_CurActorPos;

private:
	void EnableFreeCamera(bool _b);
	void CreateWeapon();
	bool OnAttacked(float _Damage);
	bool Interaction_Check(TArray<FHitResult> &_ResultArray);
	void EnableInteraction(bool _b);

public:
	// Sets default values for this character's properties
	ACharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InputAction_Move_Front(const FInputActionInstance& _Instance);
	void InputAction_Move_Right(const FInputActionInstance& _Instance);
	void InputAction_Rotation(const FInputActionInstance& _Instance);

	void InputAction_Interaction(const FInputActionInstance& _Instance);
	void InputAction_Inventory(const FInputActionInstance& _Instance);

private:

	UFUNCTION()
	void Hit_BeginOverlap(UPrimitiveComponent* _PrimitiveCom
		, AActor* _OtherActor
		, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index
		, bool _bFromSweep
		, const FHitResult& _Hit);


	UFUNCTION()
	void Interaction_BeginOverlap(UPrimitiveComponent* _PrimitiveCom
		, AActor* _OtherActor
		, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index
		, bool _bFromSweep
		, const FHitResult& _Hit);

	UFUNCTION()
	void Interaction_EndOverlap(UPrimitiveComponent* _PrimitiveCom
		, AActor* _OtherActor
		, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index);
};