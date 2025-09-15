// EGItemBase.h

#pragma once

#include "EGItemInterface.h"
#include "GameFramework/Actor.h"
#include "EGItemBase.generated.h"

class USphereComponent;

UCLASS()
class EG_API AEGItemBase : public AActor, public IEGItemInterface
{
	GENERATED_BODY()

public:
	AEGItemBase();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USphereComponent* CollisionComponent;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	virtual void OnPickUp_Implementation(AActor* PickUpActor) override;

};
