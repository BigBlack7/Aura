// Copyright YanShan University Master.BigBlack7.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// 处理玩家移动输入的函数，接收一个二维向量参数，表示玩家在水平和垂直方向上的移动输入
	void Move(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext; // 输入映射上下文，定义了输入动作和轴的映射关系

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction; // 输入动作，定义了玩家移动的输入行为
};