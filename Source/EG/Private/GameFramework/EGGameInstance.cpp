// EGGameInstance.cpp (작성자 : JJM)


#include "GameFramework/EGGameInstance.h"

#include "GameFramework/EGPlayerController.h"
#include "Blueprint/UserWidget.h" 



// (작성자 : 김세훈)
void UEGGameInstance::Init()
{
    Super::Init();

    FadeInScreenWidget = UUserWidget::CreateWidgetInstance(*this, FadeInScreenClass, NAME_None);
    FadeOutScreenWidget = UUserWidget::CreateWidgetInstance(*this, FadeOutScreenClass, NAME_None);
}

// 작성자: 김효영
#pragma region Chatting

void UEGGameInstance::SendChatMessage(const FString& Message)
{
    if (GetWorld()->GetNetMode() == NM_DedicatedServer)
    {
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            AEGPlayerController* EGPC = Cast<AEGPlayerController>(*It);
            if (EGPC)
            {
                EGPC->ClientAddChatMessage(Message);
            }
        }
    }
    
}

#pragma endregion

#pragma region LevelChange
void UEGGameInstance::RecordLevel(const FString& MapName)
{
	CurrentLevelName = MapName;
}

void UEGGameInstance::ChangeLevel()
{
    if (UWorld* World = GetWorld())
    {
        // 서버 전용 로직
        if (World->GetNetMode() == NM_DedicatedServer && !CurrentLevelName.IsEmpty())
        {
            FString Command = FString::Printf(TEXT("/Game/UI/Map/%s?listen"), *CurrentLevelName);
            World->ServerTravel(Command);
        }
    }
}

void UEGGameInstance::FadeInScreen()
{
    if (FadeInScreenWidget)
    {
        FadeInScreenWidget->AddToViewport(0);
    }
}

void UEGGameInstance::FadeOutScreen()
{
    if (FadeOutScreenWidget)
    {
        FadeOutScreenWidget->AddToViewport(0);
    }
}

#pragma endregion

void UEGGameInstance::SetFinalResults(const TArray<FFinalResult>& InResults)
{
    FinalResults = InResults;
}

TArray<FFinalResult>& UEGGameInstance::GetFinalResults()
{
    return FinalResults;
}
