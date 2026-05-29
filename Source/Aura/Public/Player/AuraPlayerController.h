// Copyright YanShan University Master.BigBlack7.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ITargetInterface;
class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// 处理玩家移动输入的函数，接收一个二维向量参数，表示玩家在水平和垂直方向上的移动输入
	void Move(const FInputActionValue& InputActionValue);

	// 处理鼠标光标追踪的函数，可能用于实现点击或悬停交互功能
	void CursorTrace();

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);

	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }
	UAuraAbilitySystemComponent* GetASC();
	void AutoRun();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext; // 输入映射上下文，定义了输入动作和轴的映射关系

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction; // 输入动作，定义了玩家移动的输入行为

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction; // 输入动作，定义了玩家移动的输入行为

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	TScriptInterface<ITargetInterface> LastTarget;
	TScriptInterface<ITargetInterface> CurrentTarget;
	FHitResult CursorHit;

	/* Click to Move */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f; // 跟随光标的时间
	float ShortPressThreshold = 0.5f; // 区别鼠标长按和短按的时间
	bool bAutoRunning = false; // 判断是否是短按时自动前进状态
	bool bTargeting = false; // 光标所在地是否有目标
	bool bShiftKeyDown = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f; // 停止自动前进时距离目的地的范围

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};