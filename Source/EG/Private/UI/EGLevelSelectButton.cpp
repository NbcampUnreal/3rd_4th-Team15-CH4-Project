#include "UI/EGLevelSelectButton.h"

#include "Components/Button.h"
#include "GameFramework/EGPlayerController.h"

void UEGLevelSelectButton::NativeConstruct()
{
    Super::NativeConstruct();
    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UEGLevelSelectButton::HandleClicked);
    }
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
            if (bRightMove && !MapName.IsEmpty())
            {
                EGPC->ServerRequestLevelRecordChange(MapName);
                EGPC->ServerRequestLevelChange();
            }
            else if (bRightMove)
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
