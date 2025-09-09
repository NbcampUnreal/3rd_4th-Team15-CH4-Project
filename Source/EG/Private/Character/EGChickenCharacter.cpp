// EGChickenCharacter.cpp (작성자 : JJM)

#include "Public/Character/EGChickenCharacter.h"

#include "EGLog.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Public/Character/Components/EGChickenMovementComponent.h"


AEGChickenCharacter::AEGChickenCharacter()
{
	PrimaryActorTick.bCanEverTick = false;			// JM : Tick 비활성화

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1080.0f, 0.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	ChickenMovementComponent = CreateDefaultSubobject<UEGChickenMovementComponent>(TEXT("ChickenMovementComponent"));
}

void AEGChickenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleMoveInput);

	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleLookInput);

	EIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EIC->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleStartSprintInput);
	EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AEGChickenCharacter::HandleStopSprintInput);
	EIC->BindAction(IA_Dash, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleDash);
	EIC->BindAction(IA_FreeLook, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleStartFreeLook);
	EIC->BindAction(IA_FreeLook, ETriggerEvent::Completed, this, &AEGChickenCharacter::HandleStopFreeLook);
}

void AEGChickenCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ChickenMovementComponent)	// JM : MovementComponent 초기화
	{
		ChickenMovementComponent->Initialize(this);
	}
	else
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
	}
	
	if (IsLocallyControlled() == true)	// JM : Client에서만 실행하여 IMC 매핑 (서버에선 실행 X)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		checkf(IsValid(PC) == true, TEXT("PlayerController is invalid."));

		UEnhancedInputLocalPlayerSubsystem* EILPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		checkf(IsValid(EILPS) == true, TEXT("EnhancedInputLocalPlayerSubsystem is invalid."));

		EILPS->AddMappingContext(IMC_Chicken, 0);
	}
}

void AEGChickenCharacter::HandleMoveInput(const FInputActionValue& InValue)
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		return;
	}
	ChickenMovementComponent->PerformMove(InValue.Get<FVector2D>());
}

void AEGChickenCharacter::HandleLookInput(const FInputActionValue& InValue)
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		return;
	}
	ChickenMovementComponent->PerformLook(InValue.Get<FVector2D>());
}

void AEGChickenCharacter::HandleStartSprintInput()
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
	}
	ChickenMovementComponent->PerformStartSprint();
}

void AEGChickenCharacter::HandleStopSprintInput()
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		return;
	}
	ChickenMovementComponent->PerformStopSprint();
}

void AEGChickenCharacter::HandleDash()
{
	if (HasAuthority())	// JM: 서버라면 바로 실행
	{
		ExecuteDash();
	}
	else				// JM: 클라라면 ServerRPC 요청 (Server Correction 과정에서 끊겨 보임)
	{
		ServerHandleDash();	
	}
}

void AEGChickenCharacter::HandleStartFreeLook()
{
	bIsFreeLooking = true;

	// 캐릭터가 회전하지 않음
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AEGChickenCharacter::HandleStopFreeLook()
{
	bIsFreeLooking = false;

	// 카메라 방향으로 캐릭터 회전
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEGChickenCharacter::ServerHandleDash_Implementation()
{
	ExecuteDash();
}

void AEGChickenCharacter::ExecuteDash()
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		return;
	}
	ChickenMovementComponent->PerformDash();
}
