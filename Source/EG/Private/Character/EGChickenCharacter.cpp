// EGChickenCharacter.cpp (작성자 : JJM)

#include "Public/Character/EGChickenCharacter.h"

#include "EGLog.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"
#include "Public/Character/Components/EGChickenMovementComponent.h"


AEGChickenCharacter::AEGChickenCharacter()
{
	PrimaryActorTick.bCanEverTick = false;			// JM : Tick 비활성화
	bReplicates = true;								// JM : Replicates 활성화
	
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
	EIC->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleAttack);
	EIC->BindAction(IA_LayEgg, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleLayEgg);
	EIC->BindAction(IA_Peck, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandlePeck);
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

void AEGChickenCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEGChickenCharacter, bIsSprinting);
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
	if (HasAuthority())	// JM: 서버라면 바로 실행
	{
		ExecuteSprint(true);
	}
	else				// JM: 클라라면 ServerRPC 요청 (Server Correction 과정에서 끊겨 보임)
	{
		ServerRPCHandleSprint(true);	
	}
}

void AEGChickenCharacter::HandleStopSprintInput()
{
	if (HasAuthority())	// JM: 서버라면 바로 실행
	{
		ExecuteSprint(false);
	}
	else				// JM: 클라라면 ServerRPC 요청 (Server Correction 과정에서 끊겨 보임)
	{
		ServerRPCHandleSprint(false);	
	}
}

void AEGChickenCharacter::HandleDash()
{
	if (HasAuthority())	// JM: 서버라면 바로 실행
	{
		ExecuteDash();
	}
	else				// JM: 클라라면 ServerRPC 요청 (Server Correction 과정에서 끊겨 보임)
	{
		ServerRPCHandleDash();	
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

void AEGChickenCharacter::HandleAttack()
{
	if (HasAuthority())	// JM : 서버라면 바로 실행
	{
		ExecuteAttack();
	}
	else				// JM : 클라라면 ServerRPC 요청
	{
		ServerRPCHandleAttack();
	}
}

void AEGChickenCharacter::HandleLayEgg()
{
	if (HasAuthority())	// JM : 서버라면 바로 실행
	{
		ExecuteLayEgg();
	}
	else				// JM : 클라라면 ServerRPC 요청
	{
		ServerRPCHandleLayEgg();
	}
}

void AEGChickenCharacter::HandlePeck()
{
	if (HasAuthority())	// JM : 서버라면 바로 실행
	{
		ExecutePeck();
	}
	else				// JM : 클라라면 ServerRPC 요청
	{
		ServerRPCHandlePeck();
	}
}

void AEGChickenCharacter::ServerRPCHandleDash_Implementation()
{
	ExecuteDash();
}

void AEGChickenCharacter::ServerRPCHandleSprint_Implementation(bool bNewIsSprint)
{
	ExecuteSprint(bNewIsSprint);
}

void AEGChickenCharacter::ServerRPCHandleAttack_Implementation()
{
	ExecuteAttack();
}

void AEGChickenCharacter::ServerRPCHandleLayEgg_Implementation()
{
	ExecuteLayEgg();
}

void AEGChickenCharacter::ServerRPCHandlePeck_Implementation()
{
	ExecutePeck();
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

void AEGChickenCharacter::ExecuteSprint(bool bNewIsSprint)
{
	bIsSprinting = bNewIsSprint;
	
	if (bNewIsSprint == true)	// 스프린트 시작
	{
		if (!ChickenMovementComponent)
		{
			EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		}
		ChickenMovementComponent->PerformStartSprint();
	}
	else						// 스프린트 종료
	{
		if (!ChickenMovementComponent)
		{
			EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		}
		ChickenMovementComponent->PerformStopSprint();
	}
}

void AEGChickenCharacter::ExecuteAttack()
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		return;
	}
	ChickenMovementComponent->PerformAttack();
}

void AEGChickenCharacter::ExecuteLayEgg()
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		return;
	}
	ChickenMovementComponent->PerformLayEgg();
}

void AEGChickenCharacter::ExecutePeck()
{
	if (!ChickenMovementComponent)
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
		return;
	}
	ChickenMovementComponent->PerformPeck();
}
