//EggActor.h

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
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

	int32 GetHealth() const;
	void SetHealth(int32 NewHealth);
	void CheckHealthAndDestroy(AActor* Actor);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateGroundState(bool bNewIsOnGround);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void PlayAbility();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	float ExplosionDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg")
	bool bIsBombEgg = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg")
	bool bIsTrickEgg = false;
	
private:
	void ApplyGravity(float DeltaTime);
	bool CheckGroundContact();
	
	int32 Health = 1;
	float GravityScale = 980.0f;
	
	UPROPERTY(Replicated)
	bool bIsOnGround = false;

};
