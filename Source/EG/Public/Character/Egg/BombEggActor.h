//BombEggActor.h

#pragma once

#include "CoreMinimal.h"
#include "Character/Egg/EggActor.h"
#include "BombEggActor.generated.h"

UCLASS()
class EG_API ABombEggActor : public AEggActor
{
	GENERATED_BODY()

// JM : 오브젝트 풀링용
public:
	ABombEggActor();
	
public:
	virtual void ApplyDamageAndCheckDestroy(int32 Damage, AActor* DamagedActor) override;

protected:
	virtual void BeginPlay() override;

	FTimerHandle ExplosionTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb")
	float ExplosionDelay = 3.0f;

private:
	void PlayAbility();

// JM : 오브젝트 풀링 적용
public:
	void StartExplosionTimer();
};
