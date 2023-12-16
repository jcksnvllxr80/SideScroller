// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollerGameModeBase.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
#include "SideScroller/MenuSystem/MainMenu.h"
#include "UObject/ConstructorHelpers.h"

/**
 * @brief Default constructor for ASideScrollerGameModeBase.
 *
 * Initializes the PrimaryActorTick and sets the DefaultPawnClass to a specified Blueprint class.
 */
ASideScrollerGameModeBase::ASideScrollerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/Blueprints/Characters/Players/BP_PC_PlayerFox")
	);
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

/**
 * @brief BeginPlay method
 *
 * This method is called when the game starts or when the level is loaded. It plays the background music at the
 * beginning of the game.
 *
 * @param None
 *
 * @return None
 */
void ASideScrollerGameModeBase::BeginPlay()
{
	/**
	Play the background music at the beginning of the game.
	*********************************************************************************/
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(AActor::GetWorld(), BackgroundMusic);
}

/**
 * Quits the game without prompting the user for confirmation.
 *
 * @param QuitPreference  The preference for quitting the game.
 */
void ASideScrollerGameModeBase::QuitGameHard() const
{
	// hard coded quit game 
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		QuitPreference,
		true
	);
}

/**
 * The Tick method is called every frame to update the game state.
 *
 * @param DeltaTime The time between the current frame and the last frame.
 */
void ASideScrollerGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (const AGameModeBase* CurrentGameMode = Cast<AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		CurrentGameMode != nullptr && (
			CurrentGameMode->GetName().Contains("MainMenu") ||
			CurrentGameMode->GetName().Contains("GameOver")
		)
	) return;  // early return if in mainmenu or gameover menu, we dont want to quit in these cases

	// PrintPlayersList();
	
	if (Players.Num() < 1)
	{
		UE_LOG(LogGameMode, Warning,
			TEXT("ASideScrollerGameModeBase::Tick - Number of Players less that 1; going to gameover menu!")
		);
		
		// go to game over screen/level where you can either start over or exit.
		if (USideScrollerGameInstance* SideScrollerGameInstance =
			Cast<USideScrollerGameInstance>(GetGameInstance())
		) {
			SideScrollerGameInstance->LoadGameOverMenu();
		} else {
			UE_LOG(LogGameMode, Warning,
				TEXT("ASideScrollerGameModeBase::Tick - Cant find sidescroller game instance; quitting game!")
			);
			QuitGameHard();
		}
	}
}

/**
 * Prints a list of players along with their status (alive or dead).
 */
void ASideScrollerGameModeBase::PrintPlayersList()
{
	if (Players.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("ASideScrollerGameModeBase::PrintPlayersList - List is empty. Returning early."));
		return;
	}
	
	FString PlayerArrayStr = "";
	for (const APC_PlayerFox* Player : Players)
	{
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("ASideScrollerGameModeBase::PrintPlayersList - Found null Player."));
			continue;
		}
		PlayerArrayStr += (Player->GetName() + (Player->IsDead() ? ": Dead; " : ": Alive; "));
	}
	UE_LOG(LogTemp, Display, TEXT("List of Players is %s"), *PlayerArrayStr);
}

/**
 * Logs out a player from the game.
 *
 * This method is called when a player controller logs out of the game. It destroys the player's character
 * (if it is a `APC_PlayerFox`), removes the player from the players array, and calls the superclass's `Logout` method.
 *
 * @param Exiting The controller for the player that is logging out.
 */
void ASideScrollerGameModeBase::Logout(AController* Exiting)
{
	APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(Exiting->GetPawn());
	if (PlayerFox != nullptr)
	{
		UE_LOG(LogGameMode, Display,
			TEXT("ASideScrollerGameModeBase::Logout - Running DestroyActor and RemovePlayer on PlayerFox, %s!"),
			*PlayerFox->GetPlayerName().ToString()
		);
		PlayerFox->DestroyActor();
		RemovePlayer(PlayerFox);
	}
	else
	{
		UE_LOG(LogGameMode, Warning,
			TEXT("ASideScrollerGameModeBase::Logout - PlayerFox is null. Not removing from Players array!")
		);
	}
	
	Super::Logout(Exiting);
}

/**
 * @brief Adds a player to the game mode.
 *
 * This method adds the given player to the game mode's list of players.
 *
 * @param PlayerFox The player to be added.
 *
 * @note The player will be added to the game mode's list of players.
 */
void ASideScrollerGameModeBase::AddPlayer(APC_PlayerFox* PlayerFox)
{
	this->Players.Add(PlayerFox);
}

/**
 * Removes a player from the Players array.
 *
 * @param PlayerFox The player to remove.
 */
void ASideScrollerGameModeBase::RemovePlayer(APC_PlayerFox* PlayerFox)
{
	if (Players.Contains(PlayerFox))
	{
		this->Players.Remove(PlayerFox);
	} else {
		UE_LOG(LogGameMode, Warning,
			TEXT("Player, %s, not in Players array. Not removing from Players array!"),
			*PlayerFox->GetPlayerName().ToString()
		);
	}
}

/**
 * Returns an array of 'APC_PlayerFox*' pointers representing the players in the game.
 *
 * @return The array of players.
 */
TArray<APC_PlayerFox*> ASideScrollerGameModeBase::GetPlayers() const
{
	return Players;
}

/**
 * @brief Travels to the Game Over Menu level.
 *
 * This method sets the bUseSeamlessTravel flag to true and performs a server travel to the Game Over Menu level.
 *
 * @param None.
 * @return None.
 */
void ASideScrollerGameModeBase::TravelToGameOverMenu()
{
	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/Maps/Map_GameOverMenu?listen");
}

/**
 * Enables the game mode input for the specified player.
 *
 * @param NewPlayer The player controller to enable the game mode input for.
 */
void ASideScrollerGameModeBase::EnablePlayerGameModeInput(APlayerController* NewPlayer)
{
	if (NewPlayer != nullptr)
	{
		UE_LOG(LogGameMode, Display, TEXT("Setting input mode for Player, %s!"), *NewPlayer->GetName());
		NewPlayer->SetShowMouseCursor(false);
		NewPlayer->SetInputMode(FInputModeGameOnly());
	}
}
