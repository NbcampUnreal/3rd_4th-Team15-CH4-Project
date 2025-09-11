// EGChickenAnimInstance.h (작성자 : JJM)

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EGChickenAnimInstance.generated.h"

class UCharacterMovementComponent;
class AEGChickenCharacter;

UCLASS()
class EG_API UEGChickenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UEGChickenAnimInstance|Character")
	TObjectPtr<AEGChickenCharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UEGChickenAnimInstance|Character")
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UEGChickenAnimInstance|Movement")
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UEGChickenAnimInstance|Movement")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UEGChickenAnimInstance|Movement")
	uint8 bShouldMove : 1;		// bool ? 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UEGChickenAnimInstance|Movement")
	uint8 bIsFalling : 1;
};
