// Copyright YanShan University Master.BigBlack7.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController==nullptr)
	{
		// 创建一个UOverlayWidgetController对象，并将它的Outer设置为当前的AAuraHUD对象。
		// Outer是Unreal Engine中对象的一个重要概念，它表示一个对象所属的父级对象。
		// 在这里，将OverlayWidgetController的Outer设置为AAuraHUD意味着OverlayWidgetController是AAuraHUD的一部分，生命周期和AAuraHUD绑定在一起。
		// 当AAuraHUD被销毁时，OverlayWidgetController也会被销毁。
		// 如果OverlayWidgetControllerClass是一个有效的类类型，那么Unreal Engine将会创建一个UOverlayWidgetController对象，并将它的Outer设置为当前的AAuraHUD对象。
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies(); // 绑定回调函数到它的依赖项，这样当相关属性发生变化时，回调函数就会被调用。
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized, please fill out BP_AuraHUD"));

	// 创建一个UUserWidget对象，使用OverlayWidgetClass指定的类类型。
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}