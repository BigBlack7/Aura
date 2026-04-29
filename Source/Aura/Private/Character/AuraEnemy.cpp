// Copyright YanShan University Master.BigBlack7.


#include "Aura/Aura.h"
#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // 启用复制，使得该组件的数据能够在网络中同步
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); // 设置复制模式为Minimal，表示只复制必要的数据，减少网络带宽的使用

	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

void AAuraEnemy::HighlightActor()
{
	// 启用自定义深度渲染，使得该组件在渲染时会被标记为一个特殊的深度值，通常用于后续的后处理效果（如轮廓描边）
	GetMesh()->SetRenderCustomDepth(true);
	// 设置自定义深度值为CUSTOM_DEPTH_STENCIL_RED，这个值通常在后处理材质中被用来识别哪些对象需要被特殊处理（如高亮显示）
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	// 初始化AbilitySystemComponent，设置它的拥有者和角色信息，使得它能够正确地管理和执行技能
	// 第一个参数是AbilitySystemComponent的拥有者，第二个参数是角色信息，通常是指这个组件所在的Actor
	// 都设置为this，表示这个组件既是拥有者也是角色信息
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}