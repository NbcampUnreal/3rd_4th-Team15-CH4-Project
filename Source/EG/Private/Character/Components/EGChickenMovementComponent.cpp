// EGChickenMovementComponent.cpp (작성자 : JJM)
#include "Public/Character/Components/EGChickenMovementComponent.h"

#include "EGLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Character/EGChickenCharacter.h"


UEGChickenMovementComponent::UEGChickenMovementComponent()
	: OwnerCharacter(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEGChickenMovementComponent::Initialize(AEGChickenCharacter* InOwner)
{
	OwnerCharacter = InOwner;
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;	// JM: 초기 속도 세팅
	}
	else
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter"));
	}
}

void UEGChickenMovementComponent::PerformMove(const FVector2D& InMovementVector)
{
	if (!OwnerCharacter || !OwnerCharacter->Controller)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter || Controller"));
		return;
	}

	if (InMovementVector.IsNearlyZero())
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("InMovementVector IsNearlyZero"));
		return;
	}
	
	const FRotator ControlRotation = OwnerCharacter->Controller->GetControlRotation();
	FRotator ControlYawRotation;
	if (OwnerCharacter->bIsFreeLooking)
	{
		ControlYawRotation = OwnerCharacter->GetActorRotation();	// 캐릭터 방향 고정
	}
	else
	{
		ControlYawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);	// 카메라 방향
	}
	
	const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

	OwnerCharacter->AddMovementInput(ForwardDirection, InMovementVector.X);
	OwnerCharacter->AddMovementInput(RightDirection, InMovementVector.Y);
	
}

void UEGChickenMovementComponent::PerformLook(const FVector2D& InLookVector)
{
	if (!OwnerCharacter || !OwnerCharacter->Controller)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter || Controller"));
		return;
	}

	OwnerCharacter->AddControllerYawInput(InLookVector.X);
	OwnerCharacter->AddControllerPitchInput(InLookVector.Y);
}

void UEGChickenMovementComponent::PerformStartSprint()
{
	if (!OwnerCharacter)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter"));
		return;
	}

	if (OwnerCharacter->HasAuthority())		// 서버에서만 실행
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void UEGChickenMovementComponent::PerformStopSprint()
{
	if (!OwnerCharacter)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter"));
		return;
	}

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UEGChickenMovementComponent::PerformDash()
{
	if (!OwnerCharacter)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter"));
		return;
	}

	if (OwnerCharacter->HasAuthority())		// 서버에서만 실행
	{
		const FVector DashDirection = OwnerCharacter->GetActorForwardVector();
		OwnerCharacter->LaunchCharacter(DashDirection * DashStrength, true, true);
	}
}


