#include "UI/EGLevelSelectButton.h"

#include "Components/Button.h"
#include "GameFramework/EGPlayerController.h"
#include "UI/DataAsset/LevelImageDataAsset.h" 
#include "Engine/Texture2D.h"

void UEGLevelSelectButton::NativeConstruct()
{
    Super::NativeConstruct();
    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UEGLevelSelectButton::HandleClicked);
    }

    UpdateMapImage();
}

void UEGLevelSelectButton::NativeDestruct()
{
    if (Button)
    {
        Button->OnClicked.RemoveDynamic(this, &UEGLevelSelectButton::HandleClicked);
    }
    Super::NativeDestruct();
}

void UEGLevelSelectButton::HandleClicked()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(PC))
        {
            if (bRightMove)
            {
                EGPC->ServerRequestLevelChange();
            }
            else
            {
                EGPC->ServerRequestLevelRecordChange(MapName);
            }
        }
    }
}

void UEGLevelSelectButton::UpdateMapImage()
{
    if (!Button || !LevelImageData || MapName.IsEmpty())
        return;

    // MapName과 같은 데이터 찾기
    FLevelImage* FoundImage = nullptr;
    for (FLevelImage& Item : LevelImageData->LevelImage)
    {
        if (Item.MapName == MapName)
        {
            FoundImage = &Item;
            break;
        }
    }

    if (!FoundImage)
        return;

    FButtonStyle NewStyle = Button->WidgetStyle;

    // Normal
    if (UTexture2D* NormalTex = FoundImage->NormalImage.LoadSynchronous())
    {
        FSlateBrush NormalBrush;
        NormalBrush.SetResourceObject(NormalTex);
        NormalBrush.ImageSize = FVector2D(256.f, 128.f); 
        FLinearColor Color(1.f, 1.f, 1.f, 1.f);
        NormalBrush.TintColor = FSlateColor(FoundImage->NormalColor);
        NewStyle.SetNormal(NormalBrush);
    }

    // Hovered
    if (UTexture2D* HoveredTex = FoundImage->HoveredImage.LoadSynchronous())
    {
        FSlateBrush HoveredBrush;
        HoveredBrush.SetResourceObject(HoveredTex);
        HoveredBrush.ImageSize = FVector2D(256.f, 128.f);
        //FLinearColor Color(0.385417f, 0.385417f, 0.385417f, 1.f);
        HoveredBrush.TintColor = FSlateColor(FoundImage->HoveredColor);
        NewStyle.SetHovered(HoveredBrush);
    }

    // Pressed
    if (UTexture2D* PressedTex = FoundImage->PressedImage.LoadSynchronous())
    {
        FSlateBrush PressedBrush;
        PressedBrush.SetResourceObject(PressedTex);
        PressedBrush.ImageSize = FVector2D(256.f, 128.f);
        //FLinearColor Color(0.088542f, 0.088542f, 0.088542f, 1.f);
        PressedBrush.TintColor = FSlateColor(FoundImage->PressedColor);
        NewStyle.SetPressed(PressedBrush);
    }

    // 적용
    Button->SetStyle(NewStyle);
}
