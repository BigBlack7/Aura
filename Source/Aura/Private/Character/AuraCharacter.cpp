// Copyright YanShan University Master.BigBlack7.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;		  // 是否根据输入旋转角色
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); // 角色旋转速率
	GetCharacterMovement()->bConstrainToPlane = true;				  // 是否约束角色在一个平面上移动
	GetCharacterMovement()->bSnapToPlaneAtStart = true;				  // 是否在开始时将角色位置调整到平面上

	bUseControllerRotationPitch = false; // 是否使用控制器的俯仰旋转来旋转角色
	bUseControllerRotationRoll = false;	 // 是否使用控制器的滚转旋转来旋转角色
	bUseControllerRotationYaw = false;	 // 是否使用控制器的偏航旋转来旋转角色
}