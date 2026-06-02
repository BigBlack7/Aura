// Copyright YanShan University Master.BigBlack7.

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	
	/** Creates a copy of this context, used to duplicate for later modifications */
    virtual FAuraGameplayEffectContext* Duplicate() const
    {
    	FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
    	*NewContext = *this;
    	if (GetHitResult())
    	{
    		// Does a deep copy of the hit result
    		NewContext->AddHitResult(*GetHitResult(), true);
    	}
    	return NewContext;
    }

	/* Getter & Setter */
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetCriticalHit(bool bNewValue) { bIsCriticalHit = bNewValue; }
	void SetBlockedHit(bool bNewValue) { bIsBlockedHit = bNewValue; }

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};