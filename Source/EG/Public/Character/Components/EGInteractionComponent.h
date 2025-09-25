// EGInteractionComponent.h

#pragma once

#include "Components/ActorComponent.h"
#include "EGInteractionComponent.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EG_API UEGInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEGInteractionComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Input")
	UInputMappingContext* IMC_Interaction;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Input")
	UInputAction* IA_Interact;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float TraceDistance = 300.f;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Interact(AActor* Target);
	
	void SetupInput();
	void HandleInteract(const FInputActionValue& Value);
	void PerformInteraction();
};
