// Copyright YanShan University Master.BigBlack7.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	/**
	* 1. MakeEffectContext()：创建一个新的GameplayEffectContextHandle，包含了关于这个效果应用的上下文信息，比如来源对象、目标对象等。
	* 2. AddSourceObject(this)：将当前的AAuraEffectActor实例添加到效果上下文中，作为这个效果的来源对象。
	* 3. MakeOutgoingSpec()：基于提供的GameplayEffectClass和效果上下文，创建一个新的GameplayEffectSpecHandle。这个SpecHandle包含了关于这个效果的详细信息，比如它的等级、持续时间、强度等。
	* 4. ApplyGameplayEffectSpecToSelf()：将创建的GameplayEffectSpec应用到目标ASC上，实际上就是将这个效果应用到目标Actor上。
	*/
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}