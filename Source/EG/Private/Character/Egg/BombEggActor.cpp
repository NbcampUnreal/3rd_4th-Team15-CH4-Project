//BombEggActor.cpp

#include "Character/Egg/BombEggActor.h"
#include "AbilitySystemComponent.h"
#include "EGLog.h"

ABombEggActor::ABombEggActor()
{
	EggType = EEggType::BombEgg;
}

void ABombEggActor::ApplyDamageAndCheckDestroy(int32 Damage, AActor* DamagedActor)
{
	Super::ApplyDamageAndCheckDestroy(Damage, DamagedActor);

	if (Health <= 0)
	{
		if (IsValid(AbilitySystemComponent) && IsValid(AbilityClass))
		{
			GetWorldTimerManager().ClearTimer(ExplosionTimer);
			PlayAbility();
		}
	}
}

void ABombEggActor::BeginPlay()
{
	Super::BeginPlay();
	
	/*DrawDebugSphere(
			GetWorld(), // 월드 포인터
			GetActorLocation(), // 중심 위치
			200, // 반지름
			32, // 세그먼트 수
			FColor::Green,
			false, // 지속적으로 그릴지 여부
			3.0f, // 표시 지속 시간 (초)
			0, // 우선순위
			1.0f);
	
	GetWorldTimerManager().SetTimer(
		ExplosionTimer,
		this,
		&ABombEggActor::PlayAbility,
		ExplosionDelay,
		false);*/
}

void ABombEggActor::PlayAbility()
{
	AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass);
}

void ABombEggActor::StartExplosionTimer()
{
	EG_LOG(LogJM, Log, TEXT("Start"));

	// 서버에서 실행시켜서 debug라인이 표시가 안되는 듯?
	DrawDebugSphere(
			GetWorld(), // 월드 포인터
			GetActorLocation(), // 중심 위치
			200, // 반지름
			32, // 세그먼트 수
			FColor::Green,
			false, // 지속적으로 그릴지 여부
			3.0f, // 표시 지속 시간 (초)
			0, // 우선순위
			1.0f);
	
	GetWorldTimerManager().SetTimer(
		ExplosionTimer,
		this,
		&ABombEggActor::PlayAbility,
		ExplosionDelay,
		false);
	
	EG_LOG(LogJM, Log, TEXT("End"));
}
