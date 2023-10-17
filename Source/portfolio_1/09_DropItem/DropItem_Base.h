// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItem_Base.generated.h"

UCLASS()
class PORTFOLIO_1_API ADropItem_Base : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowprivateAccess = "true"))
	class USphereComponent* m_Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	class UNiagaraComponent* m_DropNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowprivateAccess = "true"))
	class UProjectileMovementComponent* m_Movement;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "ItemID", meta = (AllowprivateAccess = "true"))
	EITEM_ID m_ItemID;

public:
	void SetItemID(EITEM_ID _Id) { m_ItemID = _Id; }

	EITEM_ID PickItem();

public:	
	// Sets default values for this actor's properties
	ADropItem_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
