// EGInteractionComponent.cpp

#include "Character/Components/EGInteractionComponent.h"

#include "EGLog.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Item/Interaction/EGInteractInterface.h"

UEGInteractionComponent::UEGInteractionComponent()
	: IMC_Interaction(nullptr)
	, IA_Interact(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEGInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
}

void UEGInteractionComponent::SetupInput()
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		{
			if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					if (IMC_Interaction)
					{
						Subsystem->AddMappingContext(IMC_Interaction, 0);
					}
				}
			}

			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				if (IA_Interact)
				{
					EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &UEGInteractionComponent::HandleInteract);
				}
			}
		}
	}
}

void UEGInteractionComponent::HandleInteract(const FInputActionValue& Value)
{
	PerformInteraction();
}

void UEGInteractionComponent::PerformInteraction()
{
	UE_LOG(LogTemp, Log, TEXT("Perform Interaction"));
	
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector StartLoc = Owner->GetActorLocation();
	FVector ForwardVec = Owner->GetActorForwardVector();
	FVector EndLoc = StartLoc + ForwardVec * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECC_Visibility, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->GetClass()->ImplementsInterface(UEGInteractInterface::StaticClass()))
			{
				if (GetOwner()->HasAuthority())
				{
					IEGInteractInterface::Execute_Interact(HitActor);
				}
				else
				{
					ServerRPC_Interact(HitActor);
				}
			}
		}
	}

	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 3.f, 0, 2.f);
}

void UEGInteractionComponent::ServerRPC_Interact_Implementation(AActor* Target)
{
	if (Target && Target->GetClass()->ImplementsInterface(UEGInteractInterface::StaticClass()))
	{
		EG_LOG(LogKH, Log, TEXT("Server RPC Interact Start"));
		IEGInteractInterface::Execute_Interact(Target);
		EG_LOG(LogKH, Log, TEXT("Server RPC Interact End"));
	}
}


