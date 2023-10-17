// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvenItemObject.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_1_API UInvenItemObject : public UObject
{
	GENERATED_BODY()

private:
	FString		m_IconImgPath;
	FString		m_ItemName;
	FString		m_Description;
	int32		m_Count;

public:
	void SetIconImgPath(const FString& _Path) { m_IconImgPath = _Path; }
	const FString& GetIconImgPath() { return m_IconImgPath; }

	void SetItemName(const FString& _Name) { m_ItemName = _Name; }
	const FString& GetItemName() { return m_ItemName; }

	void SetItemDescription(const FString& _Desc) { m_Description = _Desc; }
	const FString& GetItemDescription() { return m_Description; }

	void SetItemCount(int32 _Count) { m_Count = _Count; }
	int32 GetItemCount() { return m_Count; }

};
