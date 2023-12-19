// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

/**
 * Initializes the in-game menu.
 *
 * @return true if the initialization was successful, false otherwise.
 */
bool UInGameMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Cancel button during init."));
		return false;
	}
	
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Exit button during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("In Game Menu Init complete!"));
	return true;
}

/**
 * \brief This method is used to navigate back to the main menu.
 *
 * \param none
 *
 * \return void
 *
 * \note If the Menu Interface is available, it calls the LoadMainMenu() method to open the main menu.
 *       If the Menu Interface is not available, it logs an error message and returns.
 */
void UInGameMenu::BackToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OPEN MAIN MENU FAILED! Cant find the Menu Interface."));
		return;
	}
}

/**
 * @brief Resumes the game from the in-game menu.
 *
 * This method is called when the "Back to Game" button is clicked in the in-game menu.
 * It resumes the game by removing the level from the world, setting the pause state to 'false'
 * for the first player controller in the world.
 *
 * @param None.
 * @return None.
 */
void UInGameMenu::BackToGame()
{
	OnLevelRemovedFromWorld();
	// GetWorld()->GetFirstPlayerController()->SetPause(false);
}

