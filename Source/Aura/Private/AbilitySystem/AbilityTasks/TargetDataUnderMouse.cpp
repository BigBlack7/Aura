// Copyright YanShan University Master.BigBlack7.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	// NewAbilityTask是GAS提供的惯用方式（宏/模板函数），会把新对象归属到OwningAbility，并在内部为能力注册这个任务（所以生命周期由Ability管理）。
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	// 判断当前 Ability 的 Avatar/Actor 是否在本地玩家控制下（通常客户端对自己拥有的 Pawn 返回 true）。基于此分支决定是“主动发送”还是“等待别人的数据”。
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData(); // 本地控制分支：立即发送鼠标数据
	}
	else // 非本地控制分支：注册 delegate 并尝试立即调用
	{
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());

		// 如果没有立即调用（bCalledDelegate == false），说明数据还未到达，任务调用 SetWaitingOnRemotePlayerData() 把任务标记为“正在等待远端玩家数据”。
		// 这通常会让 AbilityTask 基类保持任务为挂起状态，直到 delegate 被触发或任务被取消。
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 创建预测窗口
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	// 获取鼠标光标位置信息
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// 构造目标数据句柄
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// 把目标数据发给服务器（用于网络同步/预测）
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	/* 在本地广播给Ability/监听方
	 * 客户端先收集目标并本地广播（让玩家看到即时反馈），同时把数据发送给服务器用于授权/执行真实效果。
	 * PredictionKey + Server RPC 保证客户端的本地预测和服务器端的最终决定能被关联起来，便于冲突/拒绝时回滚或确认。
	 */
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// 当 ASC 收到并分发已复制（来自客户端）的目标数据时，这个回调会被触发；它把数据从 ASC 的复制队列中“消费”掉（防止重复），
	// 然后把目标数据通过 ValidData 委托广播给本地的 Ability/蓝图，以便继续执行能力逻辑。
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}