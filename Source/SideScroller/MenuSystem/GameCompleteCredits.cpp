// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCompleteCredits.h"
#include "Components/Button.h"

/**
 * Initializes the Game Complete Credits.
 *
 * @return True if the initialization was successful, false otherwise.
 */
bool UGameCompleteCredits::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UGameCompleteCredits::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameCompleteCredits::Initialize - INIT FAILED! Cant find the MainMenu button during init.")
		);
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("UGameCompleteCredits::Initialize - GameComplete Credits Init complete!"));
	return true;
}

/**
 * @brief Method to navigate back to the main menu.
 *
 * This method is called when the user wants to go back to the main menu from the Game Complete Credits.
 * If the menu interface is available, it calls the LoadMainMenu method on the menu interface.
 * If the menu interface is not available, it logs an error message and returns.
 *
 * @param None
 * @return None
 */
void UGameCompleteCredits::BackToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameCompleteCredits::BackToMainMenu - OPEN MAIN MENU FAILED! Cant find the Menu Interface.")
		);
		return;
	}
}
