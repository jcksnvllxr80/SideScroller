// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	this->MenuInterface = _MenuInterface;
}

/**
 * Adds the menu widget to the viewport, sets up input mode, and shows the mouse cursor.
 *
 * @param None
 * @return None
 */
void UMenuWidget::Setup()
{
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(this->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMenuWidget::Setup - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMenuWidget::Setup - Cant find World obj."));
		return;
	}
}

/**
 * \brief This method is called when the owning level is removed from the world.
 * \details It is responsible for executing necessary cleanup operations for the menu widget.
 *
 * @param None
 *
 * @return None
 *
 * @note This method should be called by the engine automatically when the level is removed.
 *       It should not be called directly by the user.
 */
void UMenuWidget::OnLevelRemovedFromWorld()
{
	Super::NativeDestruct();
	this->RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			const FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = false;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMenuWidget::OnLevelRemovedFromWorld - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMenuWidget::OnLevelRemovedFromWorld - Cant find World obj."));
		return;
	}
}
