#include "UI/EGChatMessage.h"

#include "Components/TextBlock.h"

void UEGChatMessage::SetChatMessage(const FString& Message)
{
	if (ChatMessageTextBlock)
	{
		ChatMessageTextBlock->SetText(FText::FromString(Message));
		ChatMessageTextBlock->Font.Size = 16;
	}
}
