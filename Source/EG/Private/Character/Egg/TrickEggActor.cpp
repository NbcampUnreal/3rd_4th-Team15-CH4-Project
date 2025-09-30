//TrickEggActor.cpp

#include "Character/Egg/TrickEggActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/EGChickenCharacter.h"

void ATrickEggActor::ApplyDamageAndCheckDestroy(int32 Damage, AActor* DamagedActor)
{
	Super::ApplyDamageAndCheckDestroy(Damage, DamagedActor);

	if (Health <= 0)
	{
		if (AEGChickenCharacter* Player = Cast<AEGChickenCharacter>(DamagedActor))
		{
			FGameplayEventData EventData;
			EventData.EventTag = FGameplayTag::RequestGameplayTag("Event.TrickEggBroken");
			EventData.Instigator = Player;
			EventData.Target = this;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				this,
				EventData.EventTag,
				EventData);
		}
	}
}
