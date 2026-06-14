# 🔅Aura Note

## 角色👩&👾

### CharacterBase
	1.持有所有角色类的公共属性和方法并派生出主角和敌人类，包含Weapon。
	2.拥有AttributeSet和AbilitySystemComponent。
    3.使用左右手插槽以及武器插槽名来与游戏标签对应，以获得正确的攻击响应。

### Aura（主角）
	1.使用魔杖，动画蓝图为ABP_Aura，主状态机持有由速度决定的IdelWalkRun混合空间。
	2.在Server和Client都要通过AuraPlayerState设置AttributeSet和AbilitySystemComponent。
    3.通过InitAbilityActorInfo来初始化AbilitySystemComponent，OwnerActor为PlayerState，AvatarActor为AuraCharacter。
	
### Enemy
	1.共享一个ABP_Enemy动画蓝图，主状态机持有由速度决定的IdelWalkRun混合空间。
	2.使用Minimal复制模式来使用GAS。
    3.在GameMode中存储的角色属性信息表中设置敌人属性。
    4.在死亡时使用动态材质实例实现死亡时动态溶解效果。
    5.AuraAIController来操控行为树逻辑。

#### Spear Goblin & Slingshot Goblin & Shaman
	1.使用长矛和弹弓，有各自不同的派生动画蓝图。
    2.游侠使用EQS查询判定玩家位置然后选择可行位置移动，近战使用简单的距离判定。
    3.近战使用AuraMeleeAttack进行攻击。
    4.萨满使用火球攻击，并可以召唤恶魔小怪。AuraSummonAbility来实现召唤逻辑。
    5.恶魔小怪使用尾巴左右甩击，还可以使用石块投掷攻击。

### Ghoul
    1.使用双手攻击，动画蓝图为ABP_Ghoul。
    2.左右手插槽命名绑定到标签系统，攻击时根据左右手插槽名来触发不同的攻击响应。

---

## 玩家🎮

### AuraPlayController移动控制
	1.使用增强输入系统，绑定IA_Move（获取二维向量）输入映射（WASD）。
	2.根据输入值控制角色移动方向和速度，WS映射为Y轴，AD映射为X轴。
    3.AuraInputComponent派生自EnhancedInputComponent，使用BindAbilityActions函数模板来绑定输入映射到技能上。

### AuraPlayerState
	1.拥有AbilitySystemComponent和AttributeSet,并使用Mixed复制模式。

### AuraAbilitySystemComponent
    1.使用AbilityActorInfoSet()来为OnGameplayEffectAppliedDelegateToSelf添加效果应用的委托。

### AttributeSet
	1.持有玩家核心、主要、次要属性，并通过GAS进行管理和更新。
    2.通过PreAttributeChange和PostGameplayEffectExecute等函数来处理属性变化前后的逻辑，如属性上限、属性变化时的副作用等。
    3.对属性变化进行上下限处理时，PreAttributeChange仅是改变了从修改器查询到的值，并没有真正修改属性值，
    真正修改属性值是在PostGameplayEffectExecute中进行的，所以在PreAttributeChange中进行上下限处理是没有正常效果的。
    4.构造时完成TagsToAttributes，即将标签映射到属性的设置并绑定委托。

---

## Game UI💻

### AuraHUD
	1.WBP_Overlay拥有各类玩家属性UI，如生命值、魔法值等，并通过HUD显示在屏幕上。
	2.在AuraCharacter的InitAbilityActorInfo中将HUD初始化。

### AuraWidgetController
	1.不感知任何控件，负责从系统中的Model架构获取任何数据（如角色属性集）并将其广播给所有控制器为它的Widget。
    2.重写BroadcastInitialValues和BindCallbacksToDependencies函数来广播初始属性值和绑定属性变化的回调函数。

### OverlayWidgetController
	1.派生自AuraWidgetController，专门为WBP_Overlay提供数据支持。
	2.创建多个动态多播委托来广播不同属性的变化，如生命值、魔法值等。
	3.当控件蓝图能够访问控件控制器，广播属性值信息时，就可以分配一个事件来接受这些。

### AttributeMenuWidgetController
    1.派生自AuraWidgetController，专门为属性菜单提供数据支持。
    2.创建多个动态多播委托来广播不同属性的变化，如力量、敏捷等。

### AuraUserWidget
	1.单方面拥有控件控制器。派生WBP_GlobeProgressBar来制作玩家各类属性的UI显示，如生命值、魔法值等。

---

## GameEffectSystem⚡

### AuraEffectActor
	1.在游戏中表现GameplayEffect的效果，如增益效果，回复属性等。
	2.包含Instant、Duration和Periodic三种类型的GameplayEffectActor，分别对应瞬时效果、持续效果和周期性效果。
	3.对于Periodic效果，尽量不使用小数值的周期时间和效果，以避免由于网络延迟导致的同步问题。可以通过在控件上插值来平滑显示效果。
	4.效果叠加类型：StackingBehavior，决定了当同一效果多次应用时的行为，如叠加、刷新持续时间等。
	5.效果叠加-按源聚合（StackingBySource）：当同一效果多次应用时，根据来源进行叠加，
	如同一技能的多个实例可以叠加（可设置堆叠上限），但不同技能的实例不叠加。不同来源的同种效果不会相互影响。
	6.效果叠加-按目标聚合（StackingByTarget）：当同一效果多次应用时，根据目标进行叠加，
	不同源的同种效果都可以叠加（可设置堆叠上限）。同一目标的同种效果会相互影响。
	7.对于Infinite持续时间且依靠重叠触发的效果，存储该效果句柄和应用效果的角色一一映射，以便在角色离开触发区域时正确移除效果。
    8.游戏效果蓝图的修改器具体可以使用可缩放浮点数（ScalableFloat）来设置属性变化的数值，这样可以通过数据表或曲线来调整数值，提供更灵活的平衡调整，
    也可以使用基于属性的修改器（AttributeBased）来根据角色当前属性值进行计算，如根据当前生命值百分比增加伤害等，这样可以实现更动态和有趣的效果。
    9.对于AttributeBased的修改器，可以用一种属性对另一种产生效果，当有多个属性对同一属性产生效果时，修改器的执行顺序是根据
    它们在GameplayEffect中的定义顺序来决定的，先定义的修改器会先执行，计算结果将提供给下一个修改器使用。

### AuraProjectile
    1.表现技能的投射物，如火球魔法等。

---

## GAS🗡

### GameplayTags
    1.标签系统用于标记角色状态、技能类型等信息，便于在蓝图和代码中进行条件判断和逻辑处理。
    2.通过GameplayTagContainer来管理标签，可以添加、移除和查询标签。
    3.GameplayEffectAssetTag：游戏效果本身拥有但不会赋予角色的标签。
    4.GrantedTags：对应用该游戏效果的角色应用该标签。对于可堆叠效果也只会获得一次该标签。
    5.对于即时游戏效果标签没有意义因为效果应用后立刻将被移除，所以标签只会在持续时间内存在。
    6.AuraGameplayTags为全局静态类，注册角色属性大量的标签为原生标签。

### AttributeInfo
    1.UDataAsset数据资产类的派生。通过标签查找属性信息，包含属性标签、属性描述、属性值标签等信息。
    2.需要在蓝图中设置其成员变量来完成标签信息的聚合关系。

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

### Prediction机制
	1.Game Effects为Client修改属性值时，该变化会立刻在Client上反映出来（预测），同时发送请求到Server验证。
	2.Server验证后会同步结果给所有Client，不合理的变动会被检测机制回滚，确保最终状态一致合理，避免延迟过高。
    3.变量仅从服务器向客户端复制。预测键由客户端生成并发送到服务器，服务器验证后将结果复制回客户端（仅发送方）。预测键的生成和验证机制确保了客户端预测的安全性和一致性。
    4.当客户端预测性的激活一个技能时，创建一个预测窗口，在该窗口内可以发生预测性动作，期间客户端无需服务器许可即可执行改变状态的操作

### AuraGameplayAbility
    1.在AuraAbilitySystemComponent中实现初始化能力方法，在角色基类中服务器端调用该方法来为角色添加技能。
    2.Cancel Abilities with Tags：通过标签取消技能，如当角色被眩晕时取消所有攻击技能。
    3.Block Abilities with Tags：通过标签阻止技能，如当角色正在施放技能时阻止其他技能的施放。
    4.Activation Owned Tags：当技能激活时为角色添加标签，如当角色施放隐身技能时添加隐身标签。
    5.Activation Required Tags：技能激活需要角色拥有特定标签，如隐身技能需要角色拥有隐身标签才能激活。
    6.Activation Blocked Tags：技能激活被角色拥有特定标签阻止，如当角色被眩晕时拥有眩晕标签，阻止所有攻击技能的激活。
    7.Source Required Tags：技能激活需要施放者拥有特定标签，如治疗技能需要施放者拥有治疗标签才能激活。
    8.Source Blocked Tags：技能激活被施放者拥有特定标签阻止，如当施放者被沉默时拥有沉默标签，阻止所有技能的激活。
    9.Target Required Tags：技能激活需要目标拥有特定标签，如治疗技能需要目标拥有受伤标签才能激活。
    10.Target Blocked Tags：技能激活被目标拥有特定标签阻止，如攻击技能需要目标没有无敌标签才能激活。
    11.Instanced for Actor：技能实例化选项，决定了技能在激活时是否为每个角色创建一个独立的实例，还是所有角色共享同一个实例。
    12.Instanced for Execution：技能实例化选项，决定了技能在每次激活时是否创建一个新的实例（反复创建和销毁性能很低），还是在多次激活之间共享同一个实例。
    13.Non-Instanced：技能不实例化，所有角色共享同一个技能实例，适用于无状态的技能。
    14.net execution policy：Local Only（只能在本地执行，适用于视觉效果等不需要服务器验证的技能），
    Local Predicted（在本地执行并预测结果，适用于需要快速响应的技能，如攻击技能），
    Server Only（只能在服务器执行，适用于需要严格验证的技能，如治疗技能），
    Server Initiated（由服务器发起执行，适用于需要服务器控制的技能，如群体增益技能）。
    15.不需要干预Replication Policy，Server Respects Remove Ability Cancellation和Replicate Input Directly。

### AuraDamageGameplayAbility
    1.派生自AuraGameplayAbility，专门用于处理伤害技能类型。

### AbilityTask
    1.TargetDataUnderMouse：一个自定义的AbilityTask，用于获取鼠标下的目标数据，适用于需要鼠标指向的技能，如火球术。
    2.Client上激活技能后，AbilityTask会调用Activate()，稍后Server上也会调用（经过一个网络传输延迟），
    也需要通过RPC将数据传回Server进行验证和处理（也需要一个网络延时），先Activate()会导致服务器上没有有效数据。
    在技能被激活时调用ServerSetReplicatedTargetData()来将数据发送到服务器，绑定一个TargetSet Delegate来广播服务器上的数据，
    为了确保服务器上数据有效（在服务器Activate()之前就有数据到达然后被广播为无效）可以调用CallReplicatedTargetDataDelegateIfSet()来强制在服务器上调用TargetSet Delegate并检索。

### AuraSummonAbility
    1.派生自AuraGameplayAbility，用于处理Shaman召唤恶魔，在其前方分布。

### AuraAbilityTypes
    1.具有派生自FGameplayEffectContext的效果上下类。
    2.定义一系列网络系列化变量

### AuraAbilitySystemGlobals
    1.派生自UAbilitySystemGlobals，重写AllocGameplayEffectContext()来返回自定义的效果上下类。

---

## 其他功能🔧

### 默认游戏模式
	1.GameModeBase设置默认Pawn为AuraCharacter，PlayerController为AuraPlayerController。
	2.默认玩家状态为AuraPlayerState。

### 细分功能工具
	1.TargetInterface为所有可选中物体提供接口，被选中时启用描边高亮效果，通过后处理体积附加特殊材质，然后处理被选择物体的深度渲染。
    2.CombatInterface为所有可攻击物体提供接口，包含获取等级、死亡（布娃娃实现）、获取受击蒙太奇。
    3.AuraAssetManager为全局静态类，替换默认的AssetManager，提供游戏中使用的资源加载和管理功能，如技能数据表、效果蓝图等。
    4.AuraAbilitySystemLibrary派生自BlueprintFunctionLibrary，提供一些静态函数来简化GAS的使用，如获取ASC、应用效果等。

# 🌙UE5 Note
	1.TObjectPtr是一种模板指针类型，用于替代传统的裸指针（Raw Pointer）在某些场景中的使用。它通过封装指针并提供额外的功能，
	如懒加载和访问追踪，提升了编辑器模式下的性能和调试能力，同时在非编辑器模式下保持与裸指针相同的效用。
	2.对于SpringArm和Camera组件，在蓝图中实现与C++实现无性能差异。
	3.UxxxInterface给引擎看的，IxxxInterface给程序员写逻辑的。IxxxInterface作为成员变量时需要TScriptInterface包装，
	TScriptInterface会自动处理接口指针的生命周期和类型安全问题，避免了手动管理内存和类型转换的复杂性。
	4.DebugMode运行编辑器可以使用控制台显示AbilitySystem的相关信息，如主角的AttributeSet属性值等，也可以切换到场景其他目标。
    5.MakeEffectContext()创建"效果环境信息容器"，生成一个FGameplayEffectContextHandle，专门存储这个效果的 "来龙去脉"—— 即效果发生时的所有环境信息。
    MakeOutgoingSpec()创建"可执行的效果实例"，生成一个FGameplayEffectSpecHandle，它是Gameplay Effect类的"运行时实例"——把静态的GE蓝图/类和动态的上下文、等级结合起来，变成一个可以实际应用到目标上的"效果包"。
    6.UGameplayEffectExecutionCalcultion可以修改多种属性，但不能预测；及时性和周期性游戏效果只能使用其中一种；捕获这些属性不会在属性变更前运行。默认在服务器上计算执行。
    7.BlueprintPure不适用于会产生副作用的函数。
    8.行为树中按从左到右顺序执行，只有当前节点执行失败才会执行下一个，否则循环执行当前节点。
    9.GameplayCue是一个用于在游戏中表现GameplayEffect的视觉和音效反馈的系统，可以通过绑定对应标签来触发不同的Cue，如当角色受到伤害时触发受击Cue。