# ⚔Aura⚔

## 📌 项目简介

	Aura是一个基于UE5.6的GAS（Gameplay Ability System）驱动游戏项目。

---

## 🧩 目录结构

	Aura/
	│
	├── Character/
	│	├── AuraCharacterBase
	│	├── AuraCharacter
	│	└── AuraEnemy
	│
	├── Player/
	│	├── AuraPlayerState
	│	└── AuraPlayerController
	│
	├── AbilitySystem/
	│	├── Abilities/
	│	│	├── AuraProjectileSpell
	│	│	├── AuraDamageGameplayAbility
	│	│	└── AuraGameplayAbility
	│	├── AbilityTasks/
	│	│	└── TargetDataUnderMouse
	│	├── Data/
	│	│	├── CharacterClassInfo
	│	│	└── AttributeInfo
	│	├── ModMagCalc/
	│	│	├── MMC_Mana
	│	│	└── MMC_Health
	│	├── ExecCalc/
	│	│	└── ExecCalc_Damage
	│	├── AuraAbilitySystemLibrary
	│	├── AuraAbilitySystemGlobals
	│	├── AuraAttributeSet
	│	└── AuraAbilitySystemComponent
	│
	├── UI/
	│	├── Widget/
	│	│	├── DamageTextComponent
	│	│	└── AuraUserWidget
	│	├── HUD/
	│	│	└── AuraHUD
	│	└── WidgetController/
	│		├── AttributeMenuWidgetController
	│		├── OverlayWidgetController
	│		└── AuraWidgetController
	│
	├── Game/
	│	└── AuraGameModeBase
    │
	├── AI/
	│	└── AuraAIController
    │
	├── Input/
	│	├── AuraInputComponent
	│	└── AuraInputConfig
	│
	├── Interaction/
    │   ├── CombatInterface
	│	└── TargetInterface
	│
	├── Actor/
    │   ├── AuraProjectile
	│	└── AuraEffectActor
    │
    ├── AuraAssetManager
    ├── AuraAbilityTypes
    └── AuraGameplayTags