// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"

#include <sstream>

#include "GameFramework/GameStateBase.h"
#include "Misc/AssetRegistryInterface.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
#include "SideScroller/GameStates/LevelGameState.h"

/**
 * @brief Begins play for the game mode.
 *
 * This method is called when the game mode starts or when a level is loaded. It initializes
 * necessary variables and sets up any initial tasks or actions required for the game mode
 * to function properly.
 *
 * Within this method, the superclass's "BeginPlay()" method is called first to ensure proper
 * initialization of the game mode.
 *
 * After calling the superclass method, a timer is set to delay the spawning of player chosen
 * characters. The "SpawnPlayerChosenCharacters()" method is bound to this timer and will be
 * called after a specified delay. The timer will not repeat.
 */
void ALevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		this->SpawnPlayerChosenCharDelayTimerHandle,
		this,
		&ALevelGameMode::SpawnPlayerChosenCharacters,
		SpawnPlayerChosenCharDelayTimer,
		false
	);
}

/**
 * Locates the chosen character for the given player controller and spawns it in the game.
 *
 * @param Iter The iterator for the player controller.
 * @return True if the chosen character is successfully located and spawned, false otherwise.
 */
bool ALevelGameMode::LocateChosenCharacter(FConstPlayerControllerIterator Iter)
{
	APlayerController* PlayerController = Cast<APlayerController>(*Iter);
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find PlayerController.")
		)
		return true;  // start loop over cant find this player controller
	}
		
	UE_LOG(LogTemp, Display,
	       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Iterating over Players. Currently on %s."),
	       *PlayerController->GetName()	
	)
		
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find GameInstance.")
		)
		return false;  // dont go any further, cant find game instance
	}

	AGameModePlayerController* GameModePlayerController = Cast<AGameModePlayerController>(PlayerController);
	if (GameModePlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - PlayerController is not a \"GameMode\" PC.")
		)
		return true;  // start loop over this player controller is not a GameMode player controller; next!
	}
		
	const TSubclassOf<APC_PlayerFox> ChosenCharacterBP = GameInstance->GetChosenCharacter(PlayerController);
	if (ChosenCharacterBP != nullptr)
	{
		GameModePlayerController->SpawnPlayer(ChosenCharacterBP, "", PlayerController);
		UE_LOG(LogTemp, Display,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Spawning saved chosen player character.")
		)
	}
	else
	{
		if (DefaultCharacterBP != nullptr)
		{
			GameModePlayerController->SpawnPlayer(DefaultCharacterBP, "", PlayerController);
			UE_LOG(LogTemp, Display,
			       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Spawning default player character.")
			)
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - default player character is null - no spawn.")
			)
		}
	}
	return true;
}

/**
 * Spawns the player's chosen characters in the game world.
 */
void ALevelGameMode::SpawnPlayerChosenCharacters()
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find World.")
		)
		return;  // dont go any further, cant find world
	}

	for (FConstPlayerControllerIterator Iter = World->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		if (!LocateChosenCharacter(Iter)) return;
	}
}

/**
 * Start the next level in the game.
 * Retrieves the current game instance, increments the current level, and initiates a server travel to the next
 * level map.
 */
void ALevelGameMode::StartNextLevel()
{
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALevelGameMode::StartNextLevel - Can't StartNextLevel. GameInstance is null!")
		);
		return;
	}

	const ALevelGameState* CurrentGameState = Cast<ALevelGameState>(GetWorld()->GetGameState());
	if (CurrentGameState == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALevelGameMode::StartNextLevel - Can't StartNextLevel. GameState is null!")
		);
		return;
	}
	
	UE_LOG(LogTemp, Display,
		TEXT("ALevelGameMode::StartNextLevel - Finished Level %i."),
		CurrentGameState->GetCurrentLevel()
	);

	const FString GameDirectory = FString(FPaths::ProjectContentDir());
	const int NextLevel = CurrentGameState->GetCurrentLevel() + 1;
	
	if (NextLevel <= 3 && NextLevel > 0)
	{
		UWorld* World = GetWorld();
		if (!World) return;
		bUseSeamlessTravel = true;
		const FString TravelURL = FString::Printf(
			TEXT("/Game/Maps/Map_Level%i?listen"), NextLevel
		);
		World->ServerTravel(TravelURL);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALevelGameMode::StartNextLevel - All levels complete. Going to Game complete credits!")
		);
		GameInstance->LoadGameCompleteCredits();
	}
}
