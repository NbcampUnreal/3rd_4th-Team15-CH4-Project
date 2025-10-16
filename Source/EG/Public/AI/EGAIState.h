// EGAIState.h

#pragma once

#include "CoreMinimal.h"
#include "EGAIState.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),		// 대기 상태
	Move		UMETA(DisplayName = "Move"),		// 이동 상태
	Peck		UMETA(DisplayName = "Peck"),		// 모이 쪼기
	LayEgg		UMETA(DisplayName = "Lay Egg"),		// 알 낳기
	Angry		UMETA(DisplayName = "Angry"),		// 화남 상태 (공격 받을 시)
	Count		UMETA(DisplayName = "Count")		
};