// EGChickenMovementComponent.h (작성자 : JJM)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EGChickenMovementComponent.generated.h"

class AEGChickenCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EG_API UEGChickenMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEGChickenMovementComponent();

#pragma region Owner Character
public:
	void Initialize(AEGChickenCharacter* InOwner);

protected:
	UPROPERTY()
	AEGChickenCharacter* OwnerCharacter;
#pragma endregion

#pragma region Input Movement
public:
	void PerformMove(const FVector2D& InMovementVector);
	void PerformLook(const FVector2D& InLookVector);
	void PerformStartSprint();
	void PerformStopSprint();
	void PerformDash();
	void PerformAttack();
	void PerformLayEgg();
	void PerformPeck();

protected:
	UPROPERTY(EditDefaultsOnly, Category="EGChickenMovementComponent|Movement")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category="EGChickenMovementComponent|Movement")
	float SprintSpeed = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category="EGChickenMovementComponent|Movement")
	float DashStrength = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category="EGChickenMovementComponent|Movement")
	float JumpStrength = 500.0f;
#pragma endregion
	
};
