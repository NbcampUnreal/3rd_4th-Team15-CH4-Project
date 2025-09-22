// EGItem_StaminaBoost.cpp

#include "Item/ItemTypes/EGItem_StaminaBoost.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"

AEGItem_StaminaBoost::AEGItem_StaminaBoost()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AEGItem_StaminaBoost::OnPickUp_Implementation(AActor* PickUpActor)
{
	if (HasAuthority())
	{
		if (APawn* Pawn = Cast<APawn>(PickUpActor))
		{
			if (UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>())
			{
				if (StaminaBoostEffectClass)
				{
					FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
					Context.AddSourceObject(this);

					FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(StaminaBoostEffectClass, 1, Context);
					if (SpecHandle.IsValid())
					{
						ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
						EG_LOG(LogKH, Log, TEXT("%s : Stamina Boost Item Activated"), *PickUpActor->GetName());
					}
				}
			}
		}
	}
		
	
	Super::OnPickUp_Implementation(PickUpActor);
}
