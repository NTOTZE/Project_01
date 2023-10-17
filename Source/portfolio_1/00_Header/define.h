#pragma once

DECLARE_LOG_CATEGORY_EXTERN(Log_Player, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(Log_Monster, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(Log_AI, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(Log_Weapon, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(Log_UI, Log, All);


#define Log(Category, Verbosity, String)	UE_LOG(Category, Verbosity, \
											TEXT("%s"), *FString::Printf(TEXT("%s : {%d} : %s"), \
											*FString(__FUNCTION__), __LINE__, String));

