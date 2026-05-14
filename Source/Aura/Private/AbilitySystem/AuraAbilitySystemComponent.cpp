// Copyright YanShan University Master.BigBlack7.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 当该ASC收到一个效果时，绑定的函数会被调用。
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	// 任何绑定到EffectAssetTags委托的类都将收到这个TagContainer，可以在它们的逻辑中使用这些标签来决定如何响应这个效果的应用。
	EffectAssetTags.Broadcast(TagContainer);
}