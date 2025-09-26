#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGChatting.generated.h"


class UEditableText;
class UScrollBox;
class UBorder;

/**
 * 
 */
UCLASS()
class EG_API UEGChatting : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ActivateChatText();

	UPROPERTY(meta = (BindWidget))
	UEditableText* ChatText;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatScrollBox;

	UPROPERTY(meta = (BindWidget))
	UBorder* ChatBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* InputBorder;

protected:
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void Chatting(bool bChatting);
};
