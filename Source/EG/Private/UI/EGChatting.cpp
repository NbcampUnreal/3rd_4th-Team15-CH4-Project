#include "UI/EGChatting.h"

#include "GameFramework/EGPlayerController.h"
#include "Components/EditableText.h"
#include "GameFramework/PlayerState.h"
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
			// �¿� ���� ����
			FText InputText = ChatText->GetText();
			FString TrimmedText = InputText.ToString().TrimStartAndEnd();

			if (!TrimmedText.IsEmpty())
			{
				AEGPlayerController* PC = Cast<AEGPlayerController>(GetWorld()->GetFirstPlayerController());
				if (PC)
				{
					// TrimmedText �տ� UserName�� �ٿ� ���� Message ����
					APlayerState* PlayerState = PC->GetPlayerState<APlayerState>();
					FString Message = FString::Printf(TEXT("%s : %s"), *PlayerState->GetPlayerName(), *TrimmedText);
					// ä�� �޽����� ������ ���� Server RPC ȣ��
					PC->ServerSendChatMessage(Message);

					// �ٽ� FInputModeGameOnly�� ��ǲ��� ����
					FInputModeGameOnly InputMode;
					PC->SetInputMode(InputMode);

					// ä��â ���� ��Ȱ��ȭ
					ChatText->SetText(FText::GetEmpty());
					ChatText->SetIsEnabled(false);
					Chatting(false);
				}
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
