// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlayerController.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/GameModes/LevelGameMode.h"
#include "SideScroller/GameModes/LobbyGameMode.h"
#include "SideScroller/PlayerStates/PlayerFoxState.h"

/**
 * @brief Called when the game starts or when the player controller is spawned.
 *
 * This method is called when the game starts or when the player controller is spawned.
 * It is responsible for performing initialization tasks such as hiding the mouse cursor
 * and setting the input mode to game only.
 *
 * @param None
 *
 * @return None
 */
void AGameModePlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->SetShowMouseCursor(false);
	this->SetInputMode(FInputModeGameOnly());
}

/**
 * Triggers the travel to the next level.
 *
 * @param None.
 *
 * @return None.
 */
void AGameModePlayerController::TravelToLevel_Implementation()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::TravelToLevel_Implementation - No GameInstance.")
		);
		return;  // no game instance - early return
	}
	
	if (GameInstance->IsReadyToStartGame())
	{
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::TravelToLevel_Implementation - Starting game! All players ready.")
		);
		this->StartLevel();
	}
	else
	{
		UE_LOG(LogTemp, Display,
			TEXT("USelectCharacterMenu::SelectPlayer - Not starting game! All players not ready.")
		);
	}
}

/**
 * @brief Validate if the player can travel to the level.
 *
 * @return true if the validation is successful, false otherwise.
 */
bool AGameModePlayerController::TravelToLevel_Validate()
{
	return true;  // This will allow the RPC to be called
}

/**
 * Spawns a player character with the given PlayerBP, PlayerColorStr, and PlayerController.
 *
 * @param PlayerBP The blueprint class of the player character to spawn.
 * @param PlayerColorStr The color string used for logging.
 * @param PlayerController The player controller associated with the player character.
 */
void AGameModePlayerController::SpawnPlayer_Implementation(
	TSubclassOf<APC_PlayerFox> PlayerBP,
	const FString& PlayerColorStr,
	APlayerController* PlayerController
) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;
	if (PlayerBP == nullptr)
	{
		UE_LOG(LogTemp, Error,
			   TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Not spawning %s char. No %sPlayerBP."),
			   *PlayerColorStr
		);
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Not spawning %s character. Cant find World."),
			*PlayerController->GetName()
		)
		return;  // dont go any further, cant find world
	}

	USideScrollerGameInstance* SideScrollerGameInstance = Cast<USideScrollerGameInstance>(World->GetGameInstance());
	if (SideScrollerGameInstance != nullptr)
	{
		SideScrollerGameInstance->SetChosenCharacter(PlayerController, PlayerBP);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Cant save chosen char. No GameInstance")
		);
	}
	
	APawn* PlayerControllerPawn = PlayerController->GetPawn();
	if (PlayerControllerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Cant spawn new pawn. %s's pawn not found."),
			*PlayerController->GetName()
		);
		return;
	}
	
	APC_PlayerFox* NewCharacter = World->SpawnActor<APC_PlayerFox>(
		PlayerBP->GetDefaultObject()->GetClass(),
		PlayerControllerPawn->GetActorLocation() + PlayerSpawnDropInHeight,
		PlayerControllerPawn->GetActorRotation(),
		SpawnParams
	);
	
	if (NewCharacter)
	{
		APawn* PawnToBeReplaced = PlayerControllerPawn;

		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - PlayerController, %s, unpossessing old pawn"),
			*PlayerController->GetName()
		);
		PlayerController->UnPossess();

		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - PlayerController, %s, possessing new pawn"),
			*PlayerController->GetName()
		);
		PlayerController->Possess(NewCharacter);
		
		APlayerFoxState* PlayerFoxState = Cast<APlayerFoxState>(NewCharacter->GetPlayerState());
		if (PlayerFoxState != nullptr)
		{
			PlayerFoxState->SetHasChosenCharacter(true);	
		}
		
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - PlayerController, %s, destroying old Pawn"),
			*PlayerController->GetName()
		);
		if (PawnToBeReplaced)
		{
			APC_PlayerFox* PlayerPawnToBeDestroyed = Cast<APC_PlayerFox>(PawnToBeReplaced);
			if (PlayerPawnToBeDestroyed != nullptr)
			{
				PlayerPawnToBeDestroyed->DestroyActor();
			}

			PawnToBeReplaced->Destroy();
		}
	}
}

/**
 * @brief Validates the parameters for spawning a player.
 *
 * This method is invoked to validate the parameters before spawning a player in the game mode of a player controller.
 *
 * @param PlayerBP The subclass of APC_PlayerFox representing the player blueprint.
 * @param PlayerColorStr The string representing the player color.
 * @param PlayerController The pointer to the player controller instance.
 *
 * @return True if the parameters are valid; false otherwise.
 */
bool AGameModePlayerController::SpawnPlayer_Validate(
	TSubclassOf<APC_PlayerFox> PlayerBP,
	const FString& PlayerColorStr,
	APlayerController* PlayerController
) {
	return true;  // This will allow the RPC to be called
}

/**
 * Checks if the game can start based on certain requirements.
 * If the game mode is not LobbyGameMode, then a warning is logged and the method returns.
 * If the number of players requirement is not fulfilled, then a message is displayed and the method returns.
 * If the game instance cannot be found, then an error is logged and the method returns.
 * If not all players have chosen their characters, then a message is displayed and the method returns.
 * If all prerequisites are met, a message is displayed and the game instance is set to ready to start the game.
 *
 * @param None
 * @return None
 */
void AGameModePlayerController::CheckGameStartReqs_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Game mode is not LobbyGameMode.")
		)
		return;  // need more players to start - early return - dont start game
	}
	
	if (!LobbyGameMode->IsNumPlayersRequirementFulfilled())
	{
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Not enough players to start game.")
		)
		return;
	}
	
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Cant find GameInstance.")
		)
		return;
	}
	
	if (!GameInstance->IsEveryPlayersCharacterChosen())
	{
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation "
				"- Not starting game, not all players have chosen theiir characters."
			)
		)
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Player Prereqs met, game can start.")
	)
	GameInstance->SetReadyToStartGame(true);
}

/**
 * @brief Validates the game start requirements.
 *
 * This method verifies if the game start requirements are met. It should
 * be called before starting the game to ensure that all necessary conditions
 * are satisfied.
 *
 * @return Returns a boolean value indicating whether the game start
 *         requirements are valid or not. True if the requirements are valid,
 *         false otherwise.
 *
 * @see AGameModePlayerController
 */
bool AGameModePlayerController::CheckGameStartReqs_Validate()
{
	return true;  // This will allow the RPC to be called
}

/**
 * @brief Starts the level in the game.
 *
 * This method is the implementation of starting the level in the game for the current player controller. It checks if
 * the game mode is a LobbyGameMode and if not, it logs a warning message and returns without starting the game.
 * If the game mode is a LobbyGameMode, it calls the StartGame() method of the LobbyGameMode to start the game.
 *
 * @param None.
 *
 * @return None.
 *
 * @note This method assumes that the current player controller has a valid reference to the game mode.
 */
void AGameModePlayerController::StartLevel_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("AGameModePlayerController::StartLevel_Implementation - Game mode is not LobbyGameMode.")
		)
		return;  
	}

	LobbyGameMode->StartGame();
}

/**
 * @brief Validates if the start level operation can be performed.
 *
 * @return True if start level operation can be performed, false otherwise.
 */
bool AGameModePlayerController::StartLevel_Validate()
{
	return true;  // This will allow the RPC to be called
}

/**
 * @brief Starts the next level in the game.
 *
 * This method is an implementation of the StartNextLevel_Implementation function in the AGameModePlayerController
 * class. It gets the instance of the LevelGameMode and calls the StartNextLevel method on it to start the next
 * level in the game. If the game mode is not a LevelGameMode, a warning log is printed and the method returns.
 *
 * @param None
 * @return None
 */
void AGameModePlayerController::StartNextLevel_Implementation()
{
	ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(GetWorld()->GetAuthGameMode());
	if (LevelGameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("AGameModePlayerController::StartNextLevel_Implementation - Game mode is not LevelGameMode.")
		)
		return; 
	}

	LevelGameMode->StartNextLevel();
}

/**
 * @brief Validates whether the method can be called to start the next level.
 *
 * This method is used to determine if the RPC can be called to initiate the
 * start of the next level. It always returns true, allowing the RPC to be called.
 *
 * @return true if the method can be called to start the next level.
 */
bool AGameModePlayerController::StartNextLevel_Validate()
{
	return true;  // This will allow the RPC to be called
}
