#include "UI/EGChatting.h"

#include "GameFramework/EGPlayerController.h"
#include "Components/EditableText.h"
#include "GameFramework/EGPlayerState.h"
#include "Components/Border.h"


void UEGChatting::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatText)
	{
		ChatText->OnTextCommitted.AddDynamic(this, &UEGChatting::OnTextCommitted);
	}

	ChatText->SetIsEnabled(false);

	Chatting(false);
}

void UEGChatting::ActivateChatText()
{
	if (ChatText)
	{
		ChatText->SetIsEnabled(true);
		ChatText->SetFocus();

		Chatting(true);
	}
}

void UEGChatting::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (ChatText)
		{
			AEGPlayerController* PC = Cast<AEGPlayerController>(GetWorld()->GetFirstPlayerController());
			if (PC)
			{
				// 좌우 공백 제거
				FText InputText = ChatText->GetText();
				FString TrimmedText = InputText.ToString().TrimStartAndEnd();
				if (!TrimmedText.IsEmpty())
				{
					// TrimmedText 앞에 UserName을 붙여 최종 Message 생성
					AEGPlayerState* PlayerState = PC->GetPlayerState<AEGPlayerState>();
					FString Message = FString::Printf(TEXT("Player_%d : %s"), PlayerState->GetPlayerId(), *TrimmedText);
					// 채팅 메시지를 보내기 위한 Server RPC 호출
					PC->ServerSendChatMessage(Message);
				}
				
				// 다시 FInputModeGameOnly로 인풋모드 변경
				FInputModeGameOnly InputMode;
				PC->SetInputMode(InputMode);

				// 채팅창 비우고 비활성화
				ChatText->SetText(FText::GetEmpty());
				ChatText->SetIsEnabled(false);
				Chatting(false);
			}
		}
	}
}

void UEGChatting::Chatting(bool bChatting)
{
	if (bChatting)
	{
		ChatBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
		InputBorder->SetBrushColor(FLinearColor(0.020833f, 0.020833f, 0.020833f, 1.f));
		ChatText->SetHintText(FText::FromString(TEXT("Enter Text")));
	}
	else
	{
		FLinearColor TransparentColor = FLinearColor(0.f, 0.f, 0.f, 0.f);
		ChatBorder->SetBrushColor(TransparentColor);
		InputBorder->SetBrushColor(TransparentColor);
		ChatText->SetHintText(FText::FromString(TEXT("")));
	}
}
