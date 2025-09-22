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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

private:
	int32 Health = 1;
};
