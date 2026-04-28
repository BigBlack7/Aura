// Copyright YanShan University Master.BigBlack7.


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
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