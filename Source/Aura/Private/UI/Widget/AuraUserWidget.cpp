// Copyright YanShan University Master.BigBlack7.


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	/**
	* 为什么创建这个函数做这些事情？
	* 因为在蓝图中设置WidgetController时，可能会有一些额外的逻辑需要立刻执行，
	* 比如绑定事件或者更新UI状态。通过创建一个函数来设置WidgetController，
	* 我们可以在这个函数中添加这些额外的逻辑，而不是直接在蓝图中设置属性时遗漏这些重要的步骤。
	*/
	WidgetController = InWidgetController;
	WidgetControllerSet();
}