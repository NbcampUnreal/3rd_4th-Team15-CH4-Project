//TrickEggActor.h

#pragma once

#include "CoreMinimal.h"
#include "Character/Egg/EggActor.h"
#include "TrickEggActor.generated.h"

UCLASS()
class EG_API ATrickEggActor : public AEggActor
{
	GENERATED_BODY()

// JM : 오브젝트 풀링용
public:
	ATrickEggActor();
	
protected:
	virtual void ApplyDamageAndCheckDestroy(int32 Damage, AActor* DamagedActor) override;
	
};
