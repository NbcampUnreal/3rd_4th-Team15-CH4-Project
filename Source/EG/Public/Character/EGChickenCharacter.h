// EGChickenCharacter.h (작성자 : JJM)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EGChickenCharacter.generated.h"

class UEGChickenMovementComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UAbilitySystemComponent;
class UEGCharacterAttributeSet;
class UGameplayAbility;

UCLASS()
class EG_API AEGChickenCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEGChickenCharacter();
	
#pragma region ACharacter Overrides
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion

	
#pragma region Components
public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }	// JM : DedicatedX에서 가져옴. 아직 쓰진 않음
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }			// JM : DedicatedX에서 가져옴. 아직 쓰진 않음

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AEGChickenCharacter|Components")
	UEGChickenMovementComponent* ChickenMovementComponent;
#pragma endregion

	
#pragma region Input
private:
	void HandleMoveInput(const FInputActionValue& InValue);
	void HandleLookInput(const FInputActionValue& InValue);
	void HandleStartSprintInput();
	void HandleStopSprintInput();
	void HandleDash();
	void HandleStartFreeLook();
	void HandleStopFreeLook();
	void HandleAttack();
	void HandleLayEgg();
	void HandlePeck();
	
	UFUNCTION(Server, Reliable)
	void ServerRPCHandleDash();

	UFUNCTION(Server, Reliable)
	void ServerRPCHandleSprint(bool bNewIsSprint);

	UFUNCTION(Server, Reliable)
	void ServerRPCHandleAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPCHandleLayEgg();

	UFUNCTION(Server, Reliable)
	void ServerRPCHandlePeck();

	void ExecuteDash();
	void ExecuteSprint(bool bNewIsSprint);
	void ExecuteAttack();
	void ExecuteLayEgg();
	void ExecutePeck();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputMappingContext> IMC_Chicken;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_Dash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_FreeLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_LayEgg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	TObjectPtr<UInputAction> IA_Peck;

	UPROPERTY(Replicated)
	uint8 bIsSprinting:1;

public:
	UPROPERTY(BlueprintReadOnly, Category = "AEGChickenCharacter|Input")
	bool bIsFreeLooking = false;

#pragma endregion

// GAS로 스킬 사용 코드 (작성자 : 김세훈)
#pragma region GAS

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AEGChickenCharacter|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AEGChickenCharacter|GAS")
	TObjectPtr<UEGCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|GAS")
	TSubclassOf<UGameplayAbility> PeckAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AEGChickenCharacter|GAS")
	TSubclassOf<UGameplayAbility> LayEggAbilityClass;

#pragma endregion
};
