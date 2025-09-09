// EGAICharacter.h

#pragma once

#include "GameFramework/Character.h"
#include "EGAICharacter.generated.h"

UCLASS()
class EG_API AEGAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEGAICharacter();

protected:
	virtual void BeginPlay() override;
};
