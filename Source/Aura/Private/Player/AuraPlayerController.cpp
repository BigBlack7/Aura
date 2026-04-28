// Copyright YanShan University Master.BigBlack7.


#include "Player/AuraPlayerController.h"
#include "Interaction/TargetInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	// 从当前玩家的本地玩家对象中，获取“增强输入本地玩家子系统”的实例
	UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(SubSystem);

	// 将输入映射上下文添加到子系统中，优先级为0
	SubSystem->AddMappingContext(AuraContext, 0);

	// 显示鼠标光标，并设置默认的鼠标光标样式为“默认”
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;// 创建一个输入模式对象，允许同时处理游戏和UI输入
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 设置鼠标锁定行为为“不锁定”，即允许鼠标在视口外移动
	InputModeData.SetHideCursorDuringCapture(false); // 设置在临时鼠标捕获期间不隐藏光标
	SetInputMode(InputModeData); // 应用输入模式设置
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// 将MoveAction绑定到EnhancedInputComponent上，当MoveAction被触发时，调用AAuraPlayerController::Move函数
	// ETriggerEvent::Triggered表示当输入动作被触发时调用绑定的函数，可以根据需要选择不同的触发事件类型
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 从输入动作值中获取一个二维向量，表示玩家在水平和垂直方向上的移动输入
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f); // 获取当前控制旋转的偏航角，并创建一个新的旋转对象

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // 计算前进方向，基于偏航旋转
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // 计算右侧方向，基于偏航旋转

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	/**
	* 获取鼠标光标下的碰撞结果，使用可见性通道进行碰撞检测，不复杂查询
	* 为什么使用可见性通道？因为我们通常只关心玩家能看到的对象，而不是所有对象，这样可以提高性能并避免不必要的交互
	* False的原因？因为我们不需要复杂查询（如物理模拟或复杂的碰撞形状），只需要简单的碰撞检测来确定鼠标光标下的对象即可，这样可以进一步提高性能
	*/
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastTarget = CurrentTarget;
	CurrentTarget = CursorHit.GetActor();

	/**
	* 光标射线检测场景
	* A. Last Null	&& Current Null：什么都没有，保持不变
	* B. Last Null	&& Current Valid：新目标，调用HighlightActor
	* C. Last Valid && Current Null：失去目标，调用UnHighlightActor
	* D. Last Valid && Current Valid && Last != Current：切换目标，调用Last的UnHighlightActor和Current的HighlightActor
	* E. Last Valid && Current Valid && Last == Current：同一目标，保持不变
	*/
	if (LastTarget == nullptr)
	{
		if (CurrentTarget != nullptr)
		{
			// case B
			CurrentTarget->HighlightActor();
		}
		else // Current Null
		{
			// case A
		}
	}
	else // LastTarget Valid
	{
		if (CurrentTarget == nullptr)
		{
			// case C
			LastTarget->UnHighlightActor();
		}
		else // CurrentTarget Valid
		{
			if (LastTarget != CurrentTarget)
			{
				// case D
				LastTarget->UnHighlightActor();
				CurrentTarget->HighlightActor();
			}
			else
			{
				// case E
			}
		}
	}
}