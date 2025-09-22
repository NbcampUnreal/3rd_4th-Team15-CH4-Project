// EGItem_OutlineActor.cpp

#include "Item/ItemTypes/EGItem_OutlineActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EGLog.h"
#include "Abilities/GameplayAbility.h"

AEGItem_OutlineActor::AEGItem_OutlineActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEGItem_OutlineActor::OnPickUp_Implementation(AActor* PickUpActor)
{
	if (HasAuthority())
	{
		if (OutlineAbilityClass)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PickUpActor))
			{
				FGameplayAbilitySpec Spec = FGameplayAbilitySpec(OutlineAbilityClass, 1, INDEX_NONE);
				FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);

				if (Handle.IsValid())
				{
					ASC->TryActivateAbility(Handle);
					EG_LOG(LogKH, Log, TEXT("%s : Outline Player"), *PickUpActor->GetName());
				}
			}
		}
	}
	
	Super::OnPickUp_Implementation(PickUpActor);
}

