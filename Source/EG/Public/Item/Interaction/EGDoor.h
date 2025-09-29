// EGDoor.h

#pragma once

#include "GameFramework/Actor.h"
#include "EGInteractInterface.h"
#include "Components/TimelineComponent.h"
#include "EGDoor.generated.h"

class UNavModifierComponent;
class UBoxComponent;

UCLASS()
class EG_API AEGDoor : public AActor, public IEGInteractInterface
{
	GENERATED_BODY()

public:
	AEGDoor();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// IEGInteractInterface
	UFUNCTION(BlueprintCallable)
	virtual void Interact_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
	USceneComponent* Pivot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|Components")
	UNavModifierComponent* NavModifier;

	UPROPERTY()
	UTimelineComponent* DoorTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DoorCurve;

	UFUNCTION()
	void HandleDoor(float Value);

	UFUNCTION()
	void OnRep_DoorState();

private:
	FRotator OpenRotation;
	FRotator ClosedRotation;

	UPROPERTY(ReplicatedUsing = OnRep_DoorState)
	bool bIsOpen;

};
