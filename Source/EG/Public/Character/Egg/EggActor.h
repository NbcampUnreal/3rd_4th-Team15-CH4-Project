//EggActor.h

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "EEggType.h"
#include "GameFramework/Actor.h"
#include "EggActor.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;

UCLASS()
class EG_API AEggActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEggActor();

#pragma region Default

public:
	virtual void ApplyDamageAndCheckDestroy(int32 Damage, AActor* DamagedActor);

	// JM : Object Pooling에서 쓰려고 public으로 변경
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
protected:

	virtual void BeginPlay() override;

	int32 Health = 1;

	UFUNCTION()
	void OnPawnOverlap(UPrimitiveComponent* OverlappedComp,
	                   AActor* OtherActor,
	                   UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex,
	                   bool bFromSweep,
	                   const FHitResult& SweepResult);

#pragma endregion

#pragma region GAS

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> AbilityClass;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

#pragma endregion

// JM : 오브젝트 풀링용
public:
	UPROPERTY()
	EEggType EggType = EEggType::Egg;
};
