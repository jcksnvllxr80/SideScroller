// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "SideScroller/SideScrollerGameInstance.h"

/**
 * @brief Initiates the start of the game.
 *
 * This method is responsible for starting the game by:
 * 1. Retrieving the game instance from the current world.
 * 2. Setting the 'ReadyToStartGame' flag in the game instance to false.
 * 3. Logging a warning if the game instance is null and returning.
 * 4. Logging a message indicating that the lobby is being left to start the game.
 * 5. Enabling seamless travel in the current world.
 * 6. Constructing a travel URL based on the current level obtained from the game instance.
 * 7. Initiating a server travel to the constructed travel URL.
 *
 * @param None.
 * @return None.
 */
void ALobbyGameMode::StartGame()
{
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALobbyGameMode::StartGame - Can't set SetReadyToStartGame back to false. GameInstance is null!")
		);
		return;
	}
	GameInstance->SetReadyToStartGame(false);

	UE_LOG(LogTemp, Display, TEXT("Leaving lobby to start game..."));
	UWorld* World = GetWorld();
	if (!World) return;
	bUseSeamlessTravel = true;
	const FString TravelURL = FString::Printf(
		TEXT("/Game/Maps/Map_Level%i?listen"), GameInstance->GetCurrentLevel()
	);
	World->ServerTravel(TravelURL);
}

/**
 * @brief Handles the logic after a player has successfully logged in.
 *
 * This method is called after a player has successfully logged in to the game.
 * It performs the following tasks:
 * 1. Calls the Super::PostLogin method to perform any parent class PostLogin logic.
 * 2. Increments the NumberOfPlayers variable.
 * 3. Calls the SetNumPlayersToStart method to update the number of players required to start the game.
 * 4. Calls the LogPlayerCount method to log the current player count.
 * 5. Enables player input for the newly logged in player.
 * 6. Checks if the number of players is equal to or exceeds the minimum number of players required to start the game.
 *    If so, it sets the bNumPlayersRequirementFulfilled flag to true.
 *
 * @param NewPlayer A pointer to the APlayerController instance representing the newly logged in player.
 */
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	SetNumPlayersToStart();
	LogPlayerCount("PostLogin");
	EnablePlayerGameModeInput(NewPlayer);
	
	if(NumberOfPlayers >= MinPlayersToStartGame)
	{
		bNumPlayersRequirementFulfilled = true;
	}
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * This function is called when the game starts or when the ALobbyGameMode object is spawned.
 * It is an overridden function from the parent class AGameModeBase.
 *
 * @param None
 *
 * @return None
 */
void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * @brief Checks if the number of players requirement is fulfilled.
 *
 * This method checks if the number of players requirement is fulfilled
 * based on the boolean variable bNumPlayersRequirementFulfilled.
 *
 * @return True if the number of players requirement is fulfilled, false otherwise.
 */
bool ALobbyGameMode::IsNumPlayersRequirementFulfilled() const
{
	return bNumPlayersRequirementFulfilled;
}

/**
 * \brief Logout the player.
 * \param Exiting The controller that is logging out.
 *
 * This method is called when a player logs out of the game. It decreases the number of players and l
 * ogs the player count.
 */
void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
	LogPlayerCount("Logout");
}

/**
 * Sets the minimum number of players required to start the game.
 *
 * This method retrieves the number of players to start the game from the
 * current game instance and assigns it to the `MinPlayersToStartGame` variable
 * of the `ALobbyGameMode` class.
 *
 * @param None.
 *
 * @return None.
 */
void ALobbyGameMode::SetNumPlayersToStart()
{
	const USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr)
	{
		this->MinPlayersToStartGame = GameInstance->GetNumPlayersToStartGame();
	}
}

/**
 * Logs the player count in the lobby.
 *
 * @param FunctionName The name of the function calling this method.
 *
 * @note This method will log the current number of players in the lobby and the minimum number of players
 * required to start the game.
 */
void ALobbyGameMode::LogPlayerCount(const FString FunctionName) const
{
	UE_LOG(LogTemp, Display,
		   TEXT("ALobbyGameMode::%s - There are %i players in the lobby (out of %i needed)."),
		   *FunctionName,
		   NumberOfPlayers,
		   MinPlayersToStartGame
	);
}
