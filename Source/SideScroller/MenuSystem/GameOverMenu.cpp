// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverMenu.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

/**
 * Initializes the game over menu.
 *
 * @return True if the initialization was successful, false otherwise.
 */
bool UGameOverMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &UGameOverMenu::RestartGame);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::Initialize - INIT FAILED! Cant find the RestartGame button during init.")
		);
		return false;
	}
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UGameOverMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::Initialize - INIT FAILED! Cant find the MainMenu button during init.")
		);
		return false;
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UGameOverMenu::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::Initialize - INIT FAILED! Cant find the Quit button during init.")
		);
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("UGameOverMenu::Initialize - GameOver Menu Init complete!"));
	return true;
}

/**
 * @brief Method to navigate back to the main menu.
 *
 * This method is called when the user wants to go back to the main menu from the game over menu.
 * If the menu interface is available, it calls the LoadMainMenu method on the menu interface.
 * If the menu interface is not available, it logs an error message and returns.
 *
 * @param None
 * @return None
 */
void UGameOverMenu::BackToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::BackToMainMenu - OPEN MAIN MENU FAILED! Cant find the Menu Interface.")
		);
		return;
	}
}

/**
 * Restarts the game.
 *
 * @param None
 * @return None
 *
 * This method is used to restart the game. It is called when the player selects the restart option in the game over
 * menu. The method first logs a message to inform that the game is being restarted. It then gets a reference to the
 * world and checks if it is valid. If the world is valid, it sets the 'bUseSeamlessTravel' flag of the current game
 * mode to true and performs a server travel to the level 'Map_Level1'.
 */
void UGameOverMenu::RestartGame()
{
	UE_LOG(LogTemp, Display, TEXT("UGameOverMenu::RestartGame - Leaving GameOver Menu to restart game..."));
	UWorld* World = GetWorld();
	if (!World) return;
	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Map_Level1?listen");
}

/**
 * Quits the game.
 *
 * @param None.
 * @return None.
 */
void UGameOverMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			const FString QuitCommand = TEXT("quit");
			PlayerController->ConsoleCommand(QuitCommand);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UGameOverMenu::QuitGame - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGameOverMenu::QuitGame - Cant find World."));
		return;
	}
}
