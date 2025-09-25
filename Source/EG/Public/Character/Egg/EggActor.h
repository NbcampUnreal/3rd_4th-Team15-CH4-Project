//EggActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EggActor.generated.h"

UCLASS()
class EG_API AEggActor : public AActor
{
	GENERATED_BODY()

public:
	AEggActor();

	int32 GetHealth() const;
	void SetHealth(int32 NewHealth);
	void CheckHealthAndDestroy();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateGroundState(bool bNewIsOnGround);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

private:
	void ApplyGravity(float DeltaTime);
	bool CheckGroundContact();
	
	int32 Health = 1;
	float GravityScale = 980.0f;
	
	UPROPERTY(Replicated)
	bool bIsOnGround = false;
};
