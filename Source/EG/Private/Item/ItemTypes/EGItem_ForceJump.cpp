// EGItem_ForceJump.cpp

#include "Item/ItemTypes/EGItem_ForceJump.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EGLog.h"
#include "EngineUtils.h"
#include "Character/EGChickenCharacter.h"

AEGItem_ForceJump::AEGItem_ForceJump()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AEGItem_ForceJump::OnPickUp_Implementation(AActor* PickUpActor)
{
	if (HasAuthority())
	{
		if (ForceJumpAbilityClass)
		{
			for (TActorIterator<AEGChickenCharacter> It(GetWorld()); It; ++It)
			{
				AEGChickenCharacter* OtherCharacter = *It;
				if (OtherCharacter && OtherCharacter != PickUpActor)
				{
					if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherCharacter))
					{
						FGameplayAbilitySpec Spec = FGameplayAbilitySpec(ForceJumpAbilityClass, 1, INDEX_NONE);
						FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);

						if (Handle.IsValid())
						{
							ASC->TryActivateAbility(Handle);
						}
					}
				}
			}
		}
	}
	
	Super::OnPickUp_Implementation(PickUpActor);
}
