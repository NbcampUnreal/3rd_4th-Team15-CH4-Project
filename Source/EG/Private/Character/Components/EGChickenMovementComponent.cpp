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
		OwnerCharacter->GetCharacterMovement()->JumpZVelocity = JumpStrength; // 점프력 높이기 (작성자 : 김세훈)
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

	FRotator BaseRotation;
	if (OwnerCharacter->bIsFreeLooking)
	{
		// FreeLook 시작 시점의 캐릭터 회전 고정
		BaseRotation = OwnerCharacter->FreeLookBaseRotation;
	}
	else
	{
		// 일반 모드에서는 카메라 방향 사용
		const FRotator ControlRotation = OwnerCharacter->Controller->GetControlRotation();
		BaseRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
	}

	const FVector ForwardDirection = FRotationMatrix(BaseRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(BaseRotation).GetUnitAxis(EAxis::Y);

	const FVector MoveDir = (ForwardDirection * InMovementVector.X + RightDirection * InMovementVector.Y).GetSafeNormal();

	OwnerCharacter->AddMovementInput(ForwardDirection, InMovementVector.X);
	OwnerCharacter->AddMovementInput(RightDirection, InMovementVector.Y);

	// FreeLook 모드라면 이동 방향으로 회전
	if (OwnerCharacter->bIsFreeLooking && !MoveDir.IsNearlyZero())
	{
		const FRotator TargetRot = MoveDir.Rotation();
		const float RotationSpeed = OwnerCharacter->GetCharacterMovement()->RotationRate.Yaw;
		const FRotator NewRot = FMath::RInterpConstantTo(OwnerCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), RotationSpeed);
		OwnerCharacter->SetActorRotation(NewRot);
	}
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

	if (OwnerCharacter->HasAuthority())		// 서버에서만 실행 (TODO: 이미 Character에서 조건검사를 하는데 필요한가..?)
	{
		const FVector DashDirection = OwnerCharacter->GetActorForwardVector();
		OwnerCharacter->LaunchCharacter(DashDirection * DashStrength, true, true);
	}
}

void UEGChickenMovementComponent::PerformAttack()
{
	if (!OwnerCharacter)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter"));
		return;
	}

	EG_LOG_SUBOBJECT_ROLE(LogJM, Log, TEXT("Do Attack"));
}

void UEGChickenMovementComponent::PerformLayEgg()
{
	if (!OwnerCharacter)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter"));
		return;
	}

	EG_LOG_SUBOBJECT_ROLE(LogJM, Log, TEXT("Do Lay Egg"));
}

void UEGChickenMovementComponent::PerformPeck()
{
	if (!OwnerCharacter)
	{
		EG_LOG_SUBOBJECT_ROLE(LogJM, Warning, TEXT("No OwnerCharacter"));
		return;
	}

	EG_LOG_SUBOBJECT_ROLE(LogJM, Log, TEXT("Do Peck"));
}


