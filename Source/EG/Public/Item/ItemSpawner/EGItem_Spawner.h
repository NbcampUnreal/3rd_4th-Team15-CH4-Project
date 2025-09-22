// EGItem_Spawner.h

#pragma once

#include "GameFramework/Actor.h"
#include "EGItem_Spawner.generated.h"

class AEGItemBase;
class UEGItem_ConfigData;

UCLASS()
class EG_API AEGItem_Spawner : public AActor
{
	GENERATED_BODY()

public:
	AEGItem_Spawner();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSpawner|Config")
	UEGItem_ConfigData* ItemConfigData;

private:
	UPROPERTY()
	FRandomStream RandomStream;
	
	void SpawnItem();
	TSubclassOf<AEGItemBase> GetRandomItemClass();
};