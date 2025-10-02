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
#include "Components/PostProcessComponent.h"
#include "GameFramework/EGPlayerState.h"

#include "GameFramework/EGPlayerController.h"


AEGChickenCharacter::AEGChickenCharacter()
{
	PrimaryActorTick.bCanEverTick = false; // JM : Tick 비활성화
	bReplicates = true; // JM : Replicates 활성화

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
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UEGCharacterAttributeSet>(TEXT("AttributeSet"));

	// KH : 아웃라인 표시해주는 아이템을 위한 PostProcessComponent 생성
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
}

void AEGChickenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleMoveInput);

	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleLookInput);

	EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleJump); // Start로 바꾸기, Jump 함수 만들기(작성자 : 김세훈)
	EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AEGChickenCharacter::HandleStopJump);

	EIC->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleStartSprintInput); // Start로 바꾸기 (작성자 : 김세훈)
	EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AEGChickenCharacter::HandleStopSprintInput);
	EIC->BindAction(IA_Dash, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleDash); // Start로 바꾸기 (작성자 : 김세훈)
	EIC->BindAction(IA_FreeLook, ETriggerEvent::Triggered, this, &AEGChickenCharacter::HandleStartFreeLook);
	EIC->BindAction(IA_FreeLook, ETriggerEvent::Completed, this, &AEGChickenCharacter::HandleStopFreeLook);
	EIC->BindAction(IA_Attack, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleAttack); // Start로 바꾸기 (작성자 : 김세훈)
	EIC->BindAction(IA_LayEgg, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleLayEgg); // Start로 바꾸기 (작성자 : 김세훈)
	EIC->BindAction(IA_Peck, ETriggerEvent::Started, this, &AEGChickenCharacter::HandlePeck); // Start로 바꾸기 (작성자 : 김세훈)


	EIC->BindAction(IA_Chatting, ETriggerEvent::Started, this, &AEGChickenCharacter::ChatButtonPressed);	// (작성자: 김효영)
	EIC->BindAction(IA_ToggleMouse, ETriggerEvent::Started, this, &AEGChickenCharacter::ToggleMouse);	// (작성자: 김효영)

	EIC->BindAction(IA_LayBombEgg, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleLayBombEgg); 
	EIC->BindAction(IA_LayTrickEgg, ETriggerEvent::Started, this, &AEGChickenCharacter::HandleLayTrickEgg); 

}

void AEGChickenCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ChickenMovementComponent) // JM : MovementComponent 초기화
	{
		ChickenMovementComponent->Initialize(this);
	}
	else
	{
		EG_LOG_ROLE(LogJM, Warning, TEXT("No ChickenMovementComponent"));
	}

	if (IsLocallyControlled() == true) // JM : Client에서만 실행하여 IMC 매핑 (서버에선 실행 X)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		checkf(IsValid(PC) == true, TEXT("PlayerController is invalid."));

		UEnhancedInputLocalPlayerSubsystem* EILPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PC->GetLocalPlayer());
		checkf(IsValid(EILPS) == true, TEXT("EnhancedInputLocalPlayerSubsystem is invalid."));

		EILPS->AddMappingContext(IMC_Chicken, 0);
	}

	// GAS에서 처음에 꼭 해줘야 하는 Init 코드 (작성자 : 김세훈)
	if (HasAuthority())
	{
		if (IsValid(AbilitySystemComponent))
		{
			for (const auto& AbilityClass : StartupAbilities)
			{
				if (IsValid(AbilityClass))
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0, this));
					UE_LOG(LogTemp, Log, TEXT("Give Ability : %s"), *AbilityClass->GetName());
					HandleStaminaRegen();
					HandleEggEnergyRegen();
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
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}

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
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}
	
	if (IsValid(AbilitySystemComponent) && IsValid(SprintAbilityClass))
	{
		bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(SprintAbilityClass);
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("Sprint ability activated"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sprint ability failed"));
		}
	}
}

void AEGChickenCharacter::HandleStopSprintInput()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}
	
	if (IsValid(AbilitySystemComponent) && IsValid(SprintAbilityClass))
	{
		const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(SprintAbilityClass);
		if (AbilitySpec->GetAbilityInstances().Num() > 0)
		{
			AbilitySystemComponent->CancelAbilityHandle(AbilitySpec->Handle);
		}
	}
}

void AEGChickenCharacter::HandleJump()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}
	// Jump();
	ExecuteJump();
}

void AEGChickenCharacter::HandleStopJump()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}

	StopJumping();
}

void AEGChickenCharacter::HandleDash()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}
	
	ExecuteDash();
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
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}
	ExecuteAttack();
}

void AEGChickenCharacter::HandleLayEgg()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}
	
	ExecuteLayEgg();
}

void AEGChickenCharacter::HandlePeck()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}
	
	ExecutePeck();
}

void AEGChickenCharacter::HandleLayBombEgg()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}

	ExecuteLayBombEgg();
}

void AEGChickenCharacter::HandleLayTrickEgg()
{
	// Stun 태그가 있으면 움직임 불가(작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Stunned")))
		{
			return;
		}
	}

	ExecuteLayTrickEgg();
}

void AEGChickenCharacter::ExecuteDash()
{
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

void AEGChickenCharacter::ExecuteAttack()
{
	// Attack Ability 실행시키기 (작성자 : 김세훈)
	if (IsValid(AbilitySystemComponent) && IsValid(AttackAbilityClass))
	{
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag("Ability.Cooldown.Attack");
		if (!AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
		{
			bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(AttackAbilityClass);
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("Attack ability activated"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Attack ability failed (cooldown)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack Ability failed - cooldownTag having"));
		}
	}
}

void AEGChickenCharacter::ExecuteLayEgg()
{
	// LayEgg Ability 실행시키기 (작성자 : 김세훈)
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

void AEGChickenCharacter::ExecuteLayBombEgg()
{
	if (IsValid(AbilitySystemComponent) && IsValid(LayBombEggAbilityClass))
	{
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag("Ability.Cooldown.LayBombEgg");
		if (!AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
		{
			bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(LayBombEggAbilityClass);
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("LayBombEgg ability activated"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LayBombEgg ability failed (cooldown)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LayBombEgg Ability failed - cooldownTag having"));
		}
	}
}

void AEGChickenCharacter::ExecuteLayTrickEgg()
{
	if (IsValid(AbilitySystemComponent) && IsValid(LayTrickEggAbilityClass))
	{
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag("Ability.Cooldown.LayTrickEgg");
		if (!AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
		{
			bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(LayTrickEggAbilityClass);
			if (bSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("LayTrickEgg ability activated"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LayTrickEgg ability failed (cooldown)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LayTrickEgg Ability failed - cooldownTag having"));
		}
	}
}

void AEGChickenCharacter::ExecuteJump()
{
	// Jump Ability 실행시키기 (작성자 : JJM)
	if (GetMovementComponent()->IsFalling())
	{
		return;
	}
	
	if (IsValid(AbilitySystemComponent) && IsValid(JumpAbilityClass))
	{
		bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(JumpAbilityClass);
		if (bSuccess)
		{
			// EG_LOG(LogJM, Log, TEXT("Jump ability activated"));
		}
		else
		{
			// EG_LOG(LogJM, Warning, TEXT("Jump ability failed"));
		}
	}
}

// 스킬 패시브 (작성자: 김효영)
#pragma region Passive

// 스태미나 회복
void AEGChickenCharacter::HandleStaminaRegen()
{
	if (HasAuthority()) // JM : 서버라면 바로 실행
	{
		ExecuteStaminaRegen();
	}
	else // JM : 클라라면 ServerRPC 요청
	{
		ServerRPCHandleStaminaRegen();
	}
}

void AEGChickenCharacter::ServerRPCHandleStaminaRegen_Implementation()
{
	ExecuteStaminaRegen();
}

void AEGChickenCharacter::ExecuteStaminaRegen()
{
	if (AbilitySystemComponent && StaminaRegenAbilityClass)
	{
		const bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(StaminaRegenAbilityClass);
		UE_LOG(LogTemp, Log, TEXT("StaminaRegen result: %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	}
}

// 알 에너지 회복
void AEGChickenCharacter::HandleEggEnergyRegen()
{
	if (HasAuthority()) // JM : 서버라면 바로 실행
	{
		ExecuteEggEnergyRegen();
	}
	else // JM : 클라라면 ServerRPC 요청
	{
		ServerRPCHandleEggEnergyRegen();
	}
}

void AEGChickenCharacter::ServerRPCHandleEggEnergyRegen_Implementation()
{
	ExecuteEggEnergyRegen();
}

void AEGChickenCharacter::ExecuteEggEnergyRegen()
{
	if (AbilitySystemComponent && EggEnergyRegenAbilityClass)
	{
		const bool bSuccess = AbilitySystemComponent->TryActivateAbilityByClass(EggEnergyRegenAbilityClass);
		UE_LOG(LogTemp, Log, TEXT("EggEnergyRegen result: %s"), bSuccess ? TEXT("Success") : TEXT("Failed"));
	}
}

#pragma endregion

// 채팅 (작성자 : 김효영)
#pragma region Chatting
void AEGChickenCharacter::ChatButtonPressed(const FInputActionValue& Value)
{	
	AEGPlayerController* PC = Cast<AEGPlayerController>(GetController());
	if (PC)
	{
		PC->ActivateChatBox();
	}
}

void AEGChickenCharacter::ToggleMouse()
{
	AEGPlayerController* PC = Cast<AEGPlayerController>(GetController());
	if (PC)
	{
		PC->ToggleMouseCursor();
	}
}

#pragma endregion

UAbilitySystemComponent* AEGChickenCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//kms
void AEGChickenCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerState* PS = GetPlayerState())
	{
		// ASC의 OwnerActor = PlayerState, AvatarActor = 이 Character
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
}

void AEGChickenCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (APlayerState* PS = GetPlayerState())
	{
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
}
//kms