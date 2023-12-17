// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterMenu.h"
#include "Components/Button.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
#include "SideScroller/PlayerStates/PlayerFoxState.h"

/**
 * @brief Selects the pink player character.
 *
 * This method calls the SelectPlayer method with the appropriate blueprint and character name to select the
 * pink player.
 *
 * @param PinkPlayerBP The blueprint of the pink player character.
 * @param playerName The name of the pink player character.
 */
void USelectCharacterMenu::PinkPlayerSelect()
{
	SelectPlayer(PinkPlayerBP, "Pink");
}

/**
 * @brief Selects the orange player character.
 *
 * This method selects the orange player character by calling the SelectPlayer function.
 *
 * @param OrangePlayerBP The blueprint of the orange player character.
 * @param Color The color of the player character.
 */
void USelectCharacterMenu::OrangePlayerSelect()
{
	SelectPlayer(OrangePlayerBP, "Orange");
}

/**
 * @brief Function to select the yellow player character.
 *
 * This function is responsible for selecting the yellow player character in the character select menu.
 * It calls the SelectPlayer function with the necessary parameters to select the yellow player character.
 *
 * @param YellowPlayerBP The blueprint reference of the yellow player character.
 * @param PlayerName The name of the player character.
 */
void USelectCharacterMenu::YellowPlayerSelect()
{
	SelectPlayer(YellowPlayerBP, "Yellow");
}

/**
 * @brief Selects the Green player character.
 *
 * This method selects the Green player character by calling the SelectPlayer method with the GreenPlayerBP and
 * "Green" parameters.
 *
 * @param GreenPlayerBP The Blueprints of the Green player character.
 * @param color The color of the Green player character.
 *
 * @see USelectCharacterMenu::SelectPlayer()
 */
void USelectCharacterMenu::GreenPlayerSelect()
{
	SelectPlayer(GreenPlayerBP, "Green");
}

/**
 * \brief Selects the blue player character.
 * \details This method is called when the blue player is selected in the character menu.
 *          It calls the SelectPlayer method to set the blue player character.
 *
 * \param BluePlayerBP The blueprint of the blue player character to be selected.
 * \param BluePlayerTag The tag to be assigned to the blue player character.
 *
 * \see USelectCharacterMenu::SelectPlayer()
 */
void USelectCharacterMenu::BluePlayerSelect()
{
	SelectPlayer(BluePlayerBP, "Blue");
}

/**
 * @brief Selects the black player character.
 *
 * This method selects the black player character by calling the SelectPlayer method with
 * the appropriate parameters.
 *
 * @param BlackPlayerBP The blueprint of the black player character.
 */
void USelectCharacterMenu::BlackPlayerSelect()
{
	SelectPlayer(BlackPlayerBP, "Black");
}

/**
 * UpdateSelectedCharacterButtons method updates the status of selected character buttons.
 *
 * @param CharacterColorButtonMap A map containing the button color as key and the associated button as value.
 */
void USelectCharacterMenu::UpdateSelectedCharacterButtons()
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - No update. Cant find World.")
		);
		return;
	}
	
	AGameStateBase* GameState = World->GetGameState();
	if (GameState == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - No update. Cant find GameState.")
		);
		return;
	}

	TArray<FString> CurrentPlayerPawnNames;
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - Updating button statuses."));
	for (const APlayerState* PlayerStateI : GameState->PlayerArray)
	{
		const APawn* PlayerPawnI = PlayerStateI->GetPawn();
		if (PlayerPawnI == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - Cant get Pawn related to PlayerState %s."),
				*PlayerStateI->GetName()
			);
			continue;
		}
		CurrentPlayerPawnNames.Add(PlayerPawnI->GetName());
	}

	std::map<FString, UButton*>::iterator Iterator = CharacterColorButtonMap.begin();
	while (Iterator != CharacterColorButtonMap.end())
	{
		// Accessing the key
		FString ButtonColor = Iterator->first;
		// Accessing the value
		UButton* Button = Iterator->second;

		UE_LOG(LogTemp, VeryVerbose,
			TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - Enabling, the %s button associated with "
				"the %s charater. If a player is using this character, it will be disabled again."
			),
			*Button->GetName(),
			*ButtonColor
		);
		Button->SetIsEnabled(true);
		
		// look through the CurrentPlayerPawnNames to see if each button color is in the name
		for (FString PlayerPawnName : CurrentPlayerPawnNames)
		{
			if (PlayerPawnName.Contains(ButtonColor))
			{
				UE_LOG(LogTemp, Verbose,
					TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - "
						"Disabling %s as %s is using the %s charater."
					),
					*Button->GetName(),
					*PlayerPawnName,
					*ButtonColor
				);
				Button->SetIsEnabled(false);
			}
		}
		
		// iterator incremented to point next item
		++Iterator;
	}
}

/**
 * Initialize the Select Character Menu.
 *
 * @return true if initialization is successful, false otherwise.
 */
bool USelectCharacterMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;

	if (PinkPlayerButton)
	{
		PinkPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::PinkPlayerSelect);
		PinkPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		PinkPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find PinkPlayer Button."));
		return false;
	}

	if (OrangePlayerButton)
	{
		OrangePlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::OrangePlayerSelect);
		OrangePlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		OrangePlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find OrangePlayer Button."));
		return false;
	}

	if (YellowPlayerButton)
	{
		YellowPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::YellowPlayerSelect);
		YellowPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		YellowPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find YellowPlayer Button."));
		return false;
	}

	if (GreenPlayerButton)
	{
		GreenPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::GreenPlayerSelect);
		GreenPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		GreenPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find GreenPlayer Button."));
		return false;
	}

	if (BluePlayerButton)
	{
		BluePlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BluePlayerSelect);
		BluePlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		BluePlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find BluePlayer Button."));
		return false;
	}

	if (BlackPlayerButton)
	{
		BlackPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BlackPlayerSelect);
		BlackPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		BlackPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find BlackPlayer Button."));
		return false;
	}
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BackToGame);
		
		CancelButton->SetIsEnabled(false);  // disabled if the player hasn't selected a character yet
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("USelectCharacterMenu::Initialize - No PlayerController. Not enabling cancel button.")
			);
		}
		else
		{
			APC_PlayerFox* PlayerFox = dynamic_cast<APC_PlayerFox*>(PlayerController->GetPawn());
			if (PlayerFox == nullptr)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("USelectCharacterMenu::Initialize - No PlayerFox. cant check if player selcted character")
				);
			}
			else
			{
				const APlayerFoxState* PlayerFoxState = Cast<APlayerFoxState>(PlayerFox->GetPlayerState());
				if (PlayerFoxState != nullptr)
				{
					if (PlayerFoxState->GetHasChosenCharacter())
					{
						UE_LOG(LogTemp, Display,
						       TEXT("USelectCharacterMenu::Initialize - Player has selected character, "
									"enabling cancel button."
								)
						);
						CancelButton->SetIsEnabled(true);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning,
						TEXT("USelectCharacterMenu::Initialize - Cant find PlayerFoxState, wont enable cancel button.")
					);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find Cancel button."));
		return false;
	}
	
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find Exit button."));
		return false;
	}

	CharacterColorButtonMap = {
		{"Pink",			PinkPlayerButton},
		{"PC_PlayerFox_C",	OrangePlayerButton},
		{"Yellow",			YellowPlayerButton},
		{"Green",			GreenPlayerButton},
		{"Blue",			BluePlayerButton},
		{"Black",			BlackPlayerButton}
	};

	UpdateSelectedCharacterButtons();
	
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::Initialize - Select Character Menu Init complete!"));
	return true;
}

/**
 * \brief Returns to the main menu.
 *
 * This method is used to navigate back to the main menu by calling the LoadMainMenu() function of the
 * MenuInterface class.
 *
 * \param None.
 *
 * \return None.
 *
 * \see MenuInterface::LoadMainMenu()
 */
void USelectCharacterMenu::BackToMainMenu()
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
 * This method is called when the user wants to go back to the game.
 * It resumes the game by setting the pause state to false.
 *
 * @param None
 * @return None
 */
void USelectCharacterMenu::BackToGame()
{
	OnLevelRemovedFromWorld();
	GetWorld()->GetFirstPlayerController()->SetPause(false);
}

/**
 * Writes a launch warning on the screen.
 *
 * @param PlayerController The player controller to write the launch warning.
 * @return True if the launch warning was successfully written on the screen, false otherwise.
 */
bool USelectCharacterMenu::WriteLaunchWarningOnScreen(APlayerController* PlayerController)
{
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr)
	{
		UEngine* Engine = GameInstance->GetEngine();
		if (!Engine) return true;
		Engine->AddOnScreenDebugMessage(0,
            CheckStartReadinessDelayTimer,
            FColor::Green,
            TEXT("USelectCharacterMenu::SelectPlayer - Checking to see if all players are ready, if so "
                "the game will be launching in a few seconds"
            )
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("USelectCharacterMenu::SelectPlayer - Can't show onscreen message. GameInstance is null!")
		);
	}
	return false;
}

/**
 * Selects a player character based on the provided player blueprint and color string.
 *
 * @param PlayerBP The blueprint class of the player character to select.
 * @param PlayerColorStr The color string of the player character to select.
 */
void USelectCharacterMenu::SelectPlayer(const TSubclassOf<APC_PlayerFox> PlayerBP, const FString& PlayerColorStr)
{
	UE_LOG(LogTemp, Display,
		TEXT("USelectCharacterMenu::SelectPlayer - Player Selected the %s player character."),
		*PlayerColorStr
	);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("USelectCharacterMenu::SelectPlayer - Select %s Character Failed! No PlayerController."),
			*PlayerColorStr
		);
	}

	AGameModePlayerController* GameModePlayerController = Cast<AGameModePlayerController>(PlayerController);
	if (GameModePlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USelectCharacterMenu::SelectPlayer - Select %s character failed! No GameModePlayerController."),
			*PlayerColorStr
		);
		return;
	}

	GameModePlayerController->SpawnPlayer(PlayerBP, PlayerColorStr, PlayerController);
	BackToGame();

	CheckStartReadinessDelayTimerDelegate.BindUFunction(
		this,
		FName("CheckStartReadinessDelay"),
		GameModePlayerController
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		this->CheckStartReadinessDelayTimerHandle,
		CheckStartReadinessDelayTimerDelegate,
		CheckStartReadinessDelayTimer,
		false
	);

	if (WriteLaunchWarningOnScreen(PlayerController)) return;
}

/**
 * @brief CheckStartReadinessDelay method checks the readiness delay of a game mode player controller
 *        and performs required actions accordingly.
 *
 * @param GameModePlayerController The game mode player controller to check readiness delay for.
 *
 * @details This method checks the readiness delay of the specified game mode player controller.
 *          If the game mode player controller has fulfilled the necessary requirements, it will
 *          perform the required actions to start the game, such as checking game start requirements
 *          and traveling to the level.
 *          Note: This method does not return any values.
 *
 * @see AGameModePlayerController::CheckGameStartReqs()
 * @see AGameModePlayerController::TravelToLevel()
 */
void USelectCharacterMenu::CheckStartReadinessDelay(AGameModePlayerController* GameModePlayerController)
{
	GameModePlayerController->CheckGameStartReqs();
	GameModePlayerController->TravelToLevel();
}
