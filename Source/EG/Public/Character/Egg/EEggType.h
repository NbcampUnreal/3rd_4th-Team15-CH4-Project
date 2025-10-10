// EEggType.h (JJM)

#pragma once

#include "EEggType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEggType : uint8
{
	Egg			UMETA(DisplayName = "Egg"),
	BombEgg		UMETA(DisplayName = "Bomb Egg"),
	TrickEgg	UMETA(DisplayName = "Trick Egg")
};