// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../00_Header/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDoor_Base.generated.h"

UCLASS()
class PORTFOLIO_1_API ABossDoor_Base : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowprivateAccess = "true"))
	UBoxComponent* m_Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowprivateAccess = "true"))
	UStaticMeshComponent* m_Mesh;

public:	
	// Sets default values for this actor's properties
	ABossDoor_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* _PrimitiveCom
		, AActor* _OtherActor
		, UPrimitiveComponent* _OtherPrimitiveCom
		, int32 _Index
		, bool _bFromSweep
		, const FHitResult& _Hit);

};
