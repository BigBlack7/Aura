# 🔅Aura Note

## 角色👩&👾

### CharacterBase
	持有所有角色类的公共属性和方法并派生出主角和敌人类，包含Weapon。
	拥有AttributeSet和AbilitySystemComponent。

### Aura（主角）
	1.使用魔杖，动画蓝图为ABP_Aura，主状态机持有由速度决定的IdelWalkRun混合空间。
	2.在Server和Client都要通过AuraPlayerState设置AttributeSet和AbilitySystemComponent。
	
### Enemy
	1.共享一个ABP_Enemy动画蓝图，主状态机持有由速度决定的IdelWalkRun混合空间。
	2.使用Minimal复制模式来使用GAS。

#### Spear Goblin & Slingshot Goblin 
	1.使用长矛和弹弓，有各自不同的派生动画蓝图。

---

## 玩家🎮

### AuraPlayController移动控制
	1.使用增强输入系统，绑定IA_Move（获取二维向量）输入映射（WASD）。
	2.根据输入值控制角色移动方向和速度，WS映射为Y轴，AD映射为X轴。

### AuraPlayerState属性
	1.持有玩家的生命值、魔法值、等级等属性，并通过GAS进行管理和更新。
	2.拥有AbilitySystemComponent和AttributeSet,并使用Mixed复制模式。

---

## GAS🗡

### Multiplayer
	1.Server端，没有人类玩家和屏幕渲染；GameMode仅存在于此；拥有所有PlayerController；有所有PlayerState和PlayerPawn。
	2.Client端，拥有人类玩家且主机（主机即是服务器）没有延迟；仅有自己的PlayerController；有所有PlayerState和PlayerPawn；HUD和UI仅存在于此。

### Replication Mode
	1.Full：GameplayEffect复制到所有客户端。（Single Player）
	2.Mixed：GameplayEffect复制到拥有者客户端，Cues和Tags复制到所有客户端。（Multiplayer，Player-Controlled）
	3.Minimal：GameplayEffect不复制，Cues和Tags复制到所有客户端。（Multiplayer，AI-Controlled）

### Init Ability Actor Info
	1.必须在占有（Possession）之后调用（Controller已经为Pawn设置好）
	2.ASC（AbilitySystemComponent） Lives on the Pawn，Server（PossessedBy），Client（AcknowledgePossession）--- Player-Controlled
	3.ASC Lives on the PlayerState，Server（PossessedBy），Client（OnRep_PlayerState）--- Player-Controlled
	4.ASC Lives on the PlayerState，Server（BeginPlay），Client（BeginPlay）--- AI-Controlled

### Mixed模式的工作流程
	1.Controller：AuraPlayerController；OwnerActor：AuraPlayerState（拥有 ASC）；
	2.AvatarActor：AuraCharacter（放技能的人，InitAbilityActorInfo的第二个参数this）
	3.ASC：AuraPlayerState 里的 AbilitySystemComponent（用了Mixed模式）
	4.玩家在客户端按了技能键：客户端的 AuraPlayerController 收到输入。客户端的AuraCharacter先本地预测：“放技能了”（这是Mixed模式的“客户端预测”）
	5.服务器要验证并同步：服务器的 AuraPlayerState（OwnerActor）收到同步请求。服务器问AuraPlayerState：“你爸爸是谁？”（调用 AuraPlayerState->GetOwner()），
	AuraPlayerState回答：“我爸爸是AuraPlayerController！”（因为UE自动帮它SetOwner了）。服务器通过Controller找到对应的客户端，说：“对，确实放技能了，大家都同步一下”。
	6.Mixed模式要求OwnerActor->GetOwner()必须是Controller：服务器要通过Controller找到 “是哪个客户端在预测”，如果找不到，Mixed模式就失效了（要么卡，要么不同步）。
	7.敌人是AI，不需要客户端预测：移动、技能都是服务器算好的，客户端只负责 “看”，不需要本地预测，所以用Minimal模式就够了。

---

## 默认游戏模式🌏
	1.GameModeBase设置默认Pawn为AuraCharacter，PlayerController为AuraPlayerController。

---

## 细分功能工具🎯
	1. TargetInterface为所有可选中物体提供接口，被选中时启用描边高亮效果，通过后处理体积附加特殊材质，然后处理被选择物体的深度渲染。

# 🌙UE5 Note
	1.TObjectPtr是一种模板指针类型，用于替代传统的裸指针（Raw Pointer）在某些场景中的使用。它通过封装指针并提供额外的功能，
	如懒加载和访问追踪，提升了编辑器模式下的性能和调试能力，同时在非编辑器模式下保持与裸指针相同的效用。

	2.对于SpringArm和Camera组件，在蓝图中实现与C++实现无性能差异。

	3. UxxxInterface给引擎看的，IxxxInterface给程序员写逻辑的。IxxxInterface作为成员变量时需要TScriptInterface包装，
	TScriptInterface会自动处理接口指针的生命周期和类型安全问题，避免了手动管理内存和类型转换的复杂性。