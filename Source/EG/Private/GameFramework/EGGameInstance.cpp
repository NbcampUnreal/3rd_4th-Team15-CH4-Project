// EGGameInstance.cpp (작성자 : JJM)


#include "GameFramework/EGGameInstance.h"

#include "GameFramework/EGPlayerController.h"
#include "Blueprint/UserWidget.h" 



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

    if (!FadeInScreenWidget && FadeInScreenClass)
    {
        UGameViewportClient* ViewportClient = GetGameViewportClient();
        if (ViewportClient)
        {
            // Widget 생성
            FadeInScreenWidget = UUserWidget::CreateWidgetInstance(
                *this, FadeInScreenClass, NAME_None);

            if (FadeInScreenWidget)
            {
                // ViewportClient에 직접 추가
                ViewportClient->AddViewportWidgetContent(FadeInScreenWidget->TakeWidget(), 1000);
                UE_LOG(LogTemp, Warning, TEXT("LoadingScreen 표시됨"));
            }
        }
    }
}

void UEGGameInstance::FadeOutScreen()
{
    if (FadeInScreenWidget)
    {
        UGameViewportClient* ViewportClient = GetGameViewportClient();
        if (ViewportClient)
        {
            ViewportClient->RemoveViewportWidgetContent(FadeInScreenWidget->TakeWidget());
        }

        FadeInScreenWidget = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("LoadingScreen 제거됨"));
    }
    
    if (!FadeOutcreenWidget && FadeOutScreenClass)
    {
        UGameViewportClient* ViewportClient = GetGameViewportClient();
        if (ViewportClient)
        {
            // Widget 생성
            FadeOutcreenWidget = UUserWidget::CreateWidgetInstance(
                *this, FadeOutScreenClass, NAME_None);

            if (FadeOutcreenWidget)
            {
                // ViewportClient에 직접 추가
                ViewportClient->AddViewportWidgetContent(FadeOutcreenWidget->TakeWidget(), 1000);
                UE_LOG(LogTemp, Warning, TEXT("LoadingScreen 표시됨"));
            }
        }
    }
}

void UEGGameInstance::FadeOutScreenRemove()
{
    if (FadeOutcreenWidget)
    {
        UGameViewportClient* ViewportClient = GetGameViewportClient();
        if (ViewportClient)
        {
            ViewportClient->RemoveViewportWidgetContent(FadeOutcreenWidget->TakeWidget());
        }

        FadeOutcreenWidget = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("LoadingScreen 제거됨"));
    }

}

#pragma endregion
