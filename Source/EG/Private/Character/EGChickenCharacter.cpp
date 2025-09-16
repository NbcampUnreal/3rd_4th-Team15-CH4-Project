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
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"


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

	// GAS 사용을 위한 ASC와 AttributeSet 생성 (작성자 : 김세훈)
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UEGCharacterAttributeSet>(TEXT("AttributeSet"));
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
	EIC->BindAction(IA_Dash, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleDash); // Start로 바꾸기 (작성자 : 김세훈)
	EIC->BindAction(IA_FreeLook, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleStartFreeLook);
	EIC->BindAction(IA_FreeLook, ETriggerEvent::Completed, this, &AEGChickenCharacter::HandleStopFreeLook);
	EIC->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleAttack);
	EIC->BindAction(IA_LayEgg, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleLayEgg); // Start로 바꾸기 (작성자 : 김세훈)
	EIC->BindAction(IA_Peck, ETriggerEvent::Started, this, &AEGChickenCharacter::HandlePeck); // Start로 바꾸기 (작성자 : 김세훈)
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

	// GAS에서 처음에 꼭 해줘야 하는 Init 코드 (작성자 : 김세훈)
	if (HasAuthority())
	{
		if (IsValid(AbilitySystemComponent))
		{
			AbilitySystemComponent->InitAbilityActorInfo(this, this);

			for (const auto& AbilityClass : StartupAbilities)
			{
				if (IsValid(AbilityClass))
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0, this));
					UE_LOG(LogTemp, Log, TEXT("Give Ability : %s"), *AbilityClass->GetName());
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("GAS Initialized"));
		}
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
	// LayEggAbility 실행 방식 수정(작성자 : 김세훈)
	EG_LOG_ROLE(LogTemp, Log, TEXT("start"));
	
	ExecuteLayEgg();
	
	// if (HasAuthority())	// JM : 서버라면 바로 실행
	// {
	// }
	// else				// JM : 클라라면 ServerRPC 요청
	// {
	// 	//ServerRPCHandleLayEgg();
	// }
	
	EG_LOG_ROLE(LogTemp, Log, TEXT("end"));
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
	EG_LOG_ROLE(LogTemp, Log, TEXT("start"));
	ExecuteLayEgg();
	EG_LOG_ROLE(LogTemp, Log, TEXT("end"));
}

void AEGChickenCharacter::ServerRPCHandlePeck_Implementation()
{
	ExecutePeck();
}

void AEGChickenCharacter::ExecuteDash()
{
	// if (!ChickenMovementComponent)
	// {
	// 	EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
	// 	return;
	// }
	// ChickenMovementComponent->PerformDash();

	// Dash Ability 실행시키기 (작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent) && IsValid(DashAbilityClass))
	{
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag("Ability.Cooldown.Dash");
		if (!AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
		{
			bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(DashAbilityClass);
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("Dash ability activated"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Dash ability failed (cooldown)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Dash Ability failed - cooldownTag having"));
		}
	}
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
	// LayEgg Ability 실행시키기 (작성자 : 김세훈)
	EG_LOG_ROLE(LogTemp, Log, TEXT("start"));
	if (IsValid(AbilitySystemComponent) && IsValid(LayEggAbilityClass))
	{
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag("Ability.Cooldown.LayEgg");
		if (!AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
		{
			bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(LayEggAbilityClass);
			if (bSuccess)
			{
				EG_LOG_ROLE(LogTemp, Log, TEXT("LayEgg ability activated"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LayEgg ability failed (cooldown)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LayEgg Ability failed - cooldownTag having"));
		}
	}
	EG_LOG_ROLE(LogTemp, Log, TEXT("end"));
}

void AEGChickenCharacter::ExecutePeck()
{
	// Peck Ability 실행시키기 (작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent) && IsValid(PeckAbilityClass))
	{
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag("Ability.Cooldown.Peck");
		if (!AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
		{
			bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(PeckAbilityClass);
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("Peck ability activated"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Peck ability failed (cooldown)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Peck Ability failed - cooldownTag having"));
		}
	}
}
