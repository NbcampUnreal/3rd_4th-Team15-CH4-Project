// EGCharacterAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EGCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class EG_API UEGCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEGCharacterAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "AttributeData|MaxEggEnergy", ReplicatedUsing = OnRep_MaxEggEnergy)
	FGameplayAttributeData MaxEggEnergy;
	ATTRIBUTE_ACCESSORS(UEGCharacterAttributeSet, MaxEggEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "AttributeData|EggEnergy", ReplicatedUsing = OnRep_EggEnergy)
	FGameplayAttributeData EggEnergy;
	ATTRIBUTE_ACCESSORS(UEGCharacterAttributeSet, EggEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "AttributeData|MaxStamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UEGCharacterAttributeSet, MaxStamina)
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeData|Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UEGCharacterAttributeSet, Stamina)
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeData|MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UEGCharacterAttributeSet, MoveSpeed)

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_MaxEggEnergy(const FGameplayAttributeData& OldMaxEggEnergy);
	
	UFUNCTION()
	virtual void OnRep_EggEnergy(const FGameplayAttributeData& OldEggEnergy);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
};
