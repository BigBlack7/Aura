# 🔅Aura Note

## 角色设计👩&👾

### CharacterBase
	持有所有角色类的公共属性和方法并派生出主角和敌人类，包含Weapon。

### Aura（主角）
	1.使用魔杖，动画蓝图为ABP_Aura，主状态机持有由速度决定的IdelWalkRun混合空间。
	
### Enemy
	1.共享一个ABP_Enemy动画蓝图，主状态机持有由速度决定的IdelWalkRun混合空间。

#### Spear Goblin & Slingshot Goblin 
	1.使用长矛和弹弓，有各自不同的派生动画蓝图。


---

## AuraPlayController🎮

### 移动控制
	1.使用增强输入系统，绑定IA_Move（获取二维向量）输入映射（WASD）。
	2.根据输入值控制角色移动方向和速度，WS映射为Y轴，AD映射为X轴。

### TODO

# 🌙UE5 Note
	1. TObjectPtr是一种模板指针类型，用于替代传统的裸指针（Raw Pointer）在某些场景中的使用。它通过封装指针并提供额外的功能，
	如懒加载和访问追踪，提升了编辑器模式下的性能和调试能力，同时在非编辑器模式下保持与裸指针相同的效用。