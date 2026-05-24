// Copyright YanShan University Master.BigBlack7.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 确认只在服务器上调用该方法
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	// 返回能力关联的“可操作角色/Avatar”
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		// 取得生成位置并构造生成变换
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// TODO：设置投射物旋转

		// 延迟生成投射物，允许你在actor的构造脚本/BeginPlay运行前设置其属性（例如赋予一个GameplayEffect、设置初始速度、设置拥有者/instigator、设置碰撞和伤害数据等）
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: 让投射物拥有游戏效果能造成伤害

		Projectile->FinishSpawning(SpawnTransform);
	}
}