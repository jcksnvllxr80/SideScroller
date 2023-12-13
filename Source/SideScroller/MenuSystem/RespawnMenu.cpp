// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnMenu.h"

#include "Components/Button.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

/**
 * @brief Initializes the respawn menu.
 *
 * This method initializes the respawn menu by setting up button click events and checking for button references.
 * If the initialization is successful, the respawn button is disabled, and the respawn timer is started.
 *
 * @return Returns true if initialization is successful, otherwise false.
 */
bool URespawnMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (RespawnButton)
	{
		RespawnButton->OnClicked.AddDynamic(this, &URespawnMenu::Respawn);

		RespawnButton->SetIsEnabled(false);
		RunRespawnTimer();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::Initialize - INIT FAILED! Cant find the Respawn button during init.")
		);
		return false;
	}
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &URespawnMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::Initialize - INIT FAILED! Cant find the MainMenu button during init.")
		);
		return false;
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &URespawnMenu::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::Initialize - INIT FAILED! Cant find the Quit button during init.")
		);
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("URespawnMenu::Initialize - Respawn Menu Init complete!"));
	return true;
}

/**
 * @brief Returns the user to the main menu.
 *
 * This method is used to navigate back to the main menu by calling the LoadMainMenu() method from the MenuInterface.
 * If the MenuInterface is found, it will call the LoadMainMenu() method. If not found, it will log an error message
 * and return.
 *
 * @param None.
 *
 * @return None.
 */
void URespawnMenu::BackToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::BackToMainMenu - OPEN MAIN MENU FAILED! Cant find the Menu Interface.")
		);
		return;
	}
}

/**
 * Respawns the player at the last checkpoint.
 *
 * @param none
 * @return none
 */
void URespawnMenu::Respawn()
{
	if (const UWorld* World = GetWorld())
	{
		if (const APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (APawn* PawnToRespawn = PlayerController->GetPawn())
			{
				APC_PlayerFox* PlayerPawnToRespawn = Cast<APC_PlayerFox>(PawnToRespawn);
				if (PlayerPawnToRespawn != nullptr)
				{
					UE_LOG(LogTemp, Display,
						TEXT("URespawnMenu::Respawn - Player, %s, is respawning."),
						*PlayerPawnToRespawn->GetPlayerName().ToString()
					);
					PlayerPawnToRespawn->ReviveAtCheckpoint();
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("URespawnMenu::Respawn - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URespawnMenu::Respawn - Cant find World."));
		return;
	}
}

/**
 * @brief Quit the game.
 *
 * This method quits the game by executing the quit console command in the player controller.
 *
 * @param None.
 * @return None.
 *
 * @sa URespawnMenu::QuitGame()
 */
void URespawnMenu::QuitGame()
{
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			const FString QuitCommand = TEXT("quit");
			PlayerController->ConsoleCommand(QuitCommand);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("URespawnMenu::QuitGame - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URespawnMenu::QuitGame - Cant find World."));
		return;
	}
}

/**
 * Runs the respawn timer to display a countdown on the respawn menu.
 *
 * @param None
 * @return None
 */
void URespawnMenu::RunRespawnTimer()
{
	// TODO: show respawn countdown on respawn menu
	
	GetWorld()->GetTimerManager().SetTimer(
		this->RespawnDelayTimerHandle,
		this,
		&URespawnMenu::EnableRespawnButton,
		RespawnDelayTime,
		false
	);
}

/**
 * @brief Enables the respawn button.
 *
 * This method is used to enable the respawn button in the respawn menu.
 *
 * @param None.
 *
 * @return None.
 */
void URespawnMenu::EnableRespawnButton() const
{
	RespawnButton->SetIsEnabled(true);
}
