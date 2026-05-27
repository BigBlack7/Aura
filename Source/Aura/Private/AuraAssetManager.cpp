// Copyright YanShan University Master.BigBlack7.


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::InitializeNativeGameplayTags();

	// 使用TargetData必须有
	// 初始化GAS（Gameplay Ability System）的“全局运行时数据/设置”。
	// 让 Gameplay Ability System 的全局单例（UAbilitySystemGlobals）做一次启动/加载工作，
	// 确保 GAS 的全局设置、回调、复制/预测相关钩子、以及需要的默认数据在游戏运行前被正确准备好。
	UAbilitySystemGlobals::Get().InitGlobalData();
}