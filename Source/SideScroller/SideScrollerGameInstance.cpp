// Fill out your copyright notice in the Description page of Project Settings.


#include "SideScrollerGameInstance.h"

// #include <ThirdParty/CryptoPP/5.6.5/include/argnames.h>

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "GameModes/LevelGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "Online/OnlineSessionNames.h"
#include "UObject/ConstructorHelpers.h"

/**
 * @brief The name of the game session.
 *
 * This variable represents the name of the game session and it is constant.
 * The game session name is used for identifying and distinguishing different game sessions.
 *
 * @note The session name should follow certain naming conventions and guidelines specified by the game.
 *       It should be unique and easy to understand.
 *
 * @see FName
 */
const static FName SESSION_NAME = NAME_GameSession;
/**
 * @brief A constant static variable representing the session key used for storing the server name.
 *
 * This variable is of type FName and is initialized with the value "CustomServerName". It is used as the key
 * when storing and retrieving the server name from session data.
 *
 * @see FName
 */
const static FName SERVER_NAME_SESSION_KEY = TEXT("CustomServerName");

/**
 * Constructor for the USideScrollerGameInstance class.
 * Initializes the class by finding and setting the class references for various UserWidgets used in the game.
 */
USideScrollerGameInstance::USideScrollerGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!MainMenuBPClass.Class) return;
	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!InGameMenuBPClass.Class) return;
	InGameMenuClass = InGameMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> RespawnMenuBPClass(TEXT("/Game/MenuSystem/WBP_RespawnMenu"));
	if (!RespawnMenuBPClass.Class) return;
	RespawnMenuClass = RespawnMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameOverMenuBPClass(TEXT("/Game/MenuSystem/WBP_GameOverMenu"));
	if (!GameOverMenuBPClass.Class) return;
	GameOverMenuClass = GameOverMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameCompleteCreditsBPClass(
		TEXT("/Game/MenuSystem/WBP_GameCompleteCredits")
	);
	if (!GameCompleteCreditsBPClass.Class) return;
	GameCompleteCreditsClass = GameCompleteCreditsBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> SelectCharacterMenuBPClass(
		TEXT("/Game/MenuSystem/WBP_SelectCharacterMenu")
	);
	if (!SelectCharacterMenuBPClass.Class) return;
	SelectCharacterMenuClass = SelectCharacterMenuBPClass.Class;

	LoadGame();
}

/**
 * Initializes the game instance by obtaining the online subsystem and setting up session interfaces.
 */
void USideScrollerGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find IOnlineSubsystem, exiting Init early."));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Hello, got the IOnlineSubsystem, %s, from USideScrollerGameInstance Init."),
		*Subsystem->GetSubsystemName().ToString());
	SessionInterface = Subsystem->GetSessionInterface();

	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find SessionInterface, exiting Init early."));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Found the SessionInterface."));
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
		this, &USideScrollerGameInstance::OnGameSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
		this, &USideScrollerGameInstance::OnDestroySessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
		this, &USideScrollerGameInstance::OnFindSessionsComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
		this, &USideScrollerGameInstance::OnJoinSessionComplete);
}

/**
 * Loads the main menu map.
 *
 * @param None.
 * @return None.
 */
void USideScrollerGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	
	UE_LOG(LogTemp, Display, TEXT("USideScrollerGameInstance::LoadMainMenu - Loading MainMenu map."));
	PlayerController->ClientTravel("/Game/Maps/Map_MainMenu", ETravelType::TRAVEL_Absolute);
}

/**
 * Loads the Game Over menu.
 *
 * This method is responsible for loading the Game Over menu map and transitioning the game
 * from the current level to the Game Over menu. It first gets the current game mode using the
 * GetGameMode() function from the UGameplayStatics class and casts it to the AGameModeBase class.
 * If the cast is successful and the game mode is an instance of the ALevelGameMode class, it proceeds
 * to load the Game Over menu by calling the TravelToGameOverMenu() method of the LevelGameMode instance.
 * If the cast fails or the game mode is not a level game mode, a warning message is logged.
 *
 * @param None
 *
 * @return None
 */
void USideScrollerGameInstance::LoadGameOverMenu()
{
	AGameModeBase* CurrentGameMode = Cast<AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode != nullptr)
	{
		ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(CurrentGameMode);
		if (LevelGameMode != nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("USideScrollerGameInstance::LoadGameOverMenu - Loading GameOver map."));
			LevelGameMode->TravelToGameOverMenu();
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("USideScrollerGameInstance::LoadGameOverMenu - GameMode is not a level."));
		}
	}
}

/**
 * @brief Loads the GameCompleteCredits map and travels to it.
 *
 * This method is called to load the GameCompleteCredits map and travel to it.
 * It retrieves the current game mode from the world using `UGameplayStatics::GetGameMode()`
 * and checks if it is not null. Then, it casts it to an `ALevelGameMode` and checks if it is not null.
 * If it is not null, it logs the message "USideScrollerGameInstance::LoadGameCompleteCredits - Loading
 * GameCompleteCredits map." * using `UE_LOG()` with the `LogTemp` and `Display` categories. Finally,
 * it calls the `TravelToGameCompleteCredits()` * method of the `LevelGameMode`.
 * If the game mode is null or if it cannot be cast to `ALevelGameMode`, it logs the warning message
 * "USideScrollerGameInstance::LoadGameCompleteCredits - GameMode is not a level." using `UE_LOG()` with
 * the `LogTemp` and `Warning` categories.
 */
void USideScrollerGameInstance::LoadGameCompleteCredits()
{
	AGameModeBase* CurrentGameMode = Cast<AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode != nullptr)
	{
		ALevelGameMode* LevelGameMode = Cast<ALevelGameMode>(CurrentGameMode);
		if (LevelGameMode != nullptr)
		{
			UE_LOG(LogTemp, Display,
				TEXT("USideScrollerGameInstance::LoadGameCompleteCredits - Loading GameCompleteCredits map.")
			);
			LevelGameMode->TravelToGameCompleteCredits();
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("USideScrollerGameInstance::LoadGameCompleteCredits - GameMode is not a level.")
			);
		}
	}
}

/**
 * @brief Refreshes the server list by searching for active game sessions.
 *
 * This method is responsible for refreshing the server list by performing a search for active game sessions.
 * It utilizes the SessionInterface object to find sessions that match the defined search criteria.
 * The search criteria is set to search for sessions with presence enabled, to ensure that only active
 * sessions are returned.
 *
 * @note This method will log errors and return early if the SessionInterface or GameSessionSearch objects are
 * not valid.
 */
void USideScrollerGameInstance::RefreshServerList()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find SessionInterface, exiting RefreshServerList early."));
		return;
	}
	
	GameSessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!GameSessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("GameSessionSearch, is not valid! Cancel find sessions."));
		return;
	}
	// GameSessionSearch->bIsLanQuery = true;
	GameSessionSearch->MaxSearchResults = 10000;
	GameSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); 
	UE_LOG(LogTemp, Display, TEXT("Starting session search."));
	SessionInterface->FindSessions(0, GameSessionSearch.ToSharedRef());
}

/**
 * Host a game session with a specified server name.
 *
 * @param ServerName The desired name for the server.
 */
void USideScrollerGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("There is no SessionInterface, exiting Host func early."));
		return;
	}
	if (auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME); !ExistingSession)
	{
		CreateSession();
		return;
	}
	SessionInterface->DestroySession(SESSION_NAME);
}

/**
 * Joins the game session at the given IP address.
 *
 * This method is used to connect to a game session at the specified IP address.
 *
 * @param[in] IpAddress The IP address of the game session to join.
 *
 * @see UEngine::AddOnScreenDebugMessage
 * @see APlayerController::ClientTravel
 */
void USideScrollerGameInstance::JoinIP(FString& IpAddress)
{
	UEngine* Engine = GetEngine();
	if (!Engine) return;
	Engine->AddOnScreenDebugMessage(0,5,FColor::Green,
	FString::Printf(TEXT("Joining %s"), *IpAddress));
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}

/**
 * Joins a session with the specified index.
 *
 * @param Index The index of the session to join.
 *
 * @return void
 *
 * This method joins a game session using the specified index. It first checks if the session interface is valid.
 * If it is, it checks if the game session search object is also valid. If it is, the method calls the JoinSession
 * function of the session interface with the parameters 0, SESSION_NAME, and the search results at the specified index.
 * If the game session search object is not valid, an error message is logged and the method returns. If the session
 * interface is not valid, an error message is logged and the method returns.
 */
void USideScrollerGameInstance::Join(uint32 Index)
{
	if (SessionInterface.IsValid())
	{
		if (GameSessionSearch.IsValid())
		{
			SessionInterface->JoinSession(0, SESSION_NAME, GameSessionSearch->SearchResults[Index]);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game session search is not valid, cant join a server."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Session interface is not valid, cant join a server."));
		return;
	}

}

/**
 * Loads and sets up the main menu widget.
 *
 * @param None.
 * @return None.
 */
void USideScrollerGameInstance::LoadMenu()
{
	if (MainMenuClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Main Menu blueprint class %s."), *MainMenuClass->GetName());
		Menu = CreateWidget<UMainMenu>(this, MainMenuClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMainMenu Menu from main menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the Main Menu blueprint class."));
		return;
	}
	
}

/**
 * @brief Function to load the in-game menu widget.
 *
 * This function checks if the in-game menu class is set. If it is,
 * it creates an instance of the menu widget and sets up the menu interface.
 *
 * @param None
 *
 * @return None
 */
void USideScrollerGameInstance::InGameLoadMenu()
{
	if (InGameMenuClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found InGame Menu blueprint class %s."), *InGameMenuClass->GetName());
		if (UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass))
		{
			InGameMenu->Setup();
			InGameMenu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMenuWidget Menu from InGame menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the InGame Menu blueprint class."));
		return;
	}
}

/**
 * @brief Respawn and load the menu.
 *
 * This method is responsible for respawning the player character and loading the respawn menu.
 * If the respawn menu blueprint class is found, it creates an instance of the UMenuWidget and sets up the menu
 * interface. If the respawn menu blueprint class is not found, an error message will be logged.
 *
 * @param None.
 * @return None.
 */
void USideScrollerGameInstance::RespawnLoadMenu()
{
	if (RespawnMenuClass)
	{
		UE_LOG(LogTemp, Display,
			TEXT("USideScrollerGameInstance::RespawnLoadMenu - Found Respawn Menu blueprint class %s."),
			*RespawnMenuClass->GetName()
		);
		if (UMenuWidget* RespawnMenu = CreateWidget<UMenuWidget>(this, RespawnMenuClass))
		{
			RespawnMenu->Setup();
			RespawnMenu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error,
				TEXT("USideScrollerGameInstance::RespawnLoadMenu - Cant create UMenuWidget Menu from "
					"Respawn menu blueprint class.")
			)
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("USideScrollerGameInstance::RespawnLoadMenu - Cant find the Respawn Menu blueprint class.")
		)
		return;
	}
}

/**
 * Selects the character load menu.
 *
 * The method creates a SelectCharacterMenu widget and sets up its properties.
 * It also assigns this instance as the menu interface for the SelectCharacterMenu.
 *
 * @param none
 *
 * @return none
 */
void USideScrollerGameInstance::SelectCharacterLoadMenu()
{
	if (SelectCharacterMenuClass)
	{
		UE_LOG(LogTemp, Display,
			TEXT("Found SelectCharacter Menu blueprint class %s."),
			*SelectCharacterMenuClass->GetName()
		);
		if (UMenuWidget* SelectCharacterMenu = CreateWidget<UMenuWidget>(this, SelectCharacterMenuClass))
		{
			SelectCharacterMenu->Setup();
			SelectCharacterMenu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMenuWidget Menu from SelectCharacter menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the SelectCharacter Menu blueprint class."));
		return;
	}
}

/**
 * Load the game over menu.
 *
 * If the GameOverMenuClass is set, this method will create an instance of the UMenuWidget using the GameOverMenuClass
 * and set it up by calling its Setup() method. It will also set the UMenuInterface of the GameOverMenu to this
 * game instance.
 *
 * @param None
 * @return None
 */
void USideScrollerGameInstance::GameOverLoadMenu()
{
	if (GameOverMenuClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found GameOver Menu blueprint class %s."), *GameOverMenuClass->GetName());
		if (UMenuWidget* GameOverMenu = CreateWidget<UMenuWidget>(this, GameOverMenuClass))
		{
			GameOverMenu->Setup();
			GameOverMenu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMenuWidget Menu from GameOver menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the GameOver Menu blueprint class."));
		return;
	}
}

/**
 * Load the credits screen when the game is completed.
 */
void USideScrollerGameInstance::GameCompleteLoadCredits()
{
	if (GameCompleteCreditsClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found GameComplete Credits blueprint class %s."), *GameCompleteCreditsClass->GetName());
		if (UMenuWidget* GameCompleteCredits = CreateWidget<UMenuWidget>(this, GameCompleteCreditsClass))
		{
			GameCompleteCredits->Setup();
			GameCompleteCredits->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMenuWidget Menu from GameComplete Credits blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the GameComplete Credits blueprint class."));
		return;
	}
}

/**
 * @brief Returns the number of players required to start the game.
 *
 * @return The number of players required to start the game.
 */
int USideScrollerGameInstance::GetNumPlayersToStartGame() const
{
	return NumPlayers;
}

/**
 * Retrieves the chosen character for the given player controller.
 *
 * @param PlayerController The player controller for which to retrieve the chosen character.
 *
 * @return The TSubclassOf<APC_PlayerFox> representing the chosen character for the player controller.
 *         Returns nullptr if either the PlayerControllerChosenCharMap is empty, the player pawn is not a PlayerFox,
 *         or the player could not be found in the PlayerControllerChosenCharMap.
 */
TSubclassOf<APC_PlayerFox> USideScrollerGameInstance::GetChosenCharacter(APlayerController* PlayerController)
{
	if (PlayerControllerChosenCharMap.empty())
	{
		UE_LOG(LogTemp, Display,
			TEXT("USideScrollerGameInstance::GetChosenCharacter - PlayerControllerChosenCharMap is empty.")
		)
	} else {
		const APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(PlayerController->GetPawn());
		if (PlayerFox == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("USideScrollerGameInstance::GetChosenCharacter - PlayerPawn is not a PlayerFox.")
			)
			return nullptr;
		}
		
		const auto ChosenCharacter = PlayerControllerChosenCharMap.find(PlayerFox->GetPlayerName().ToString());
		if (ChosenCharacter == PlayerControllerChosenCharMap.end())
		{
			UE_LOG(LogTemp, Display,
				TEXT("USideScrollerGameInstance::GetChosenCharacter - Cant find Player, %s, in the map."),
				*PlayerFox->GetPlayerName().ToString()
			)
			return nullptr;
		}
		return ChosenCharacter->second;
	}
	return nullptr;
}

/**
 * Sets the chosen character for a given player controller.
 *
 * @param PlayerController The player controller for which to set the chosen character.
 * @param ChosenCharacter The class of the chosen character.
 */
void USideScrollerGameInstance::SetChosenCharacter(
	APlayerController* PlayerController,
	TSubclassOf<APC_PlayerFox> ChosenCharacter
) {
	const APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(PlayerController->GetPawn());
	if (PlayerFox == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USideScrollerGameInstance::SetChosenCharacter - PlayerPawn is not a PlayerFox.")
		)
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("USideScrollerGameInstance::SetChosenCharacter - Setting %s's chosen character as %s."),
		*PlayerFox->GetPlayerName().ToString(),
		*ChosenCharacter->GetName()
	)
	
	this->PlayerControllerChosenCharMap.insert({PlayerFox->GetPlayerName().ToString(), ChosenCharacter});
}

/**
 * @brief Checks if every player has chosen a character.
 *
 * @return True if every player has chosen a character, false otherwise.
 *
 * @param None.
 */
bool USideScrollerGameInstance::IsEveryPlayersCharacterChosen() const
{
	return (this->PlayerControllerChosenCharMap.size() >= GetNumPlayersToStartGame()) ? true : false;
}

/**
 * \brief Sets the flag indicating whether the game is ready to start
 *
 * This method sets the flag indicating whether the game is ready to start.
 * When the game is ready to start, the flag is set to true; otherwise, it
 * is set to false.
 *
 * \param bCond The value to set the flag to. Pass true to indicate that
 *              the game is ready to start, or false to indicate that it
 *              is not.
 *
 * \remarks This method should be called when the game is ready to transition
 *          from the lobby or preparation phase to the actual gameplay phase.
 *          The flag should be set to true when all necessary initialization
 *          has been completed and the game is in a suitable state to start.
 */
void USideScrollerGameInstance::SetReadyToStartGame(bool bCond)
{
	this->bReadyToStartGame = bCond;
}

/**
 * @brief Checks if the game is ready to start.
 *
 * @return true if the game is ready to start, false otherwise.
 */
bool USideScrollerGameInstance::IsReadyToStartGame()
{
	return this->bReadyToStartGame;
}

/**
 * Callback function for when the game session is completed.
 *
 * This function is called after the game session has completed for the given session name.
 *
 * @param SessionName The name of the game session that has completed.
 * @param Success Indicates whether the game session completed successfully or not.
 *
 * @return None
 */
void USideScrollerGameInstance::OnGameSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Not able to create a session."));
		return;
	}
	UEngine* Engine = GetEngine();
	if (!Engine) return;
	Engine->AddOnScreenDebugMessage(0,2,FColor::Green,TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!World) return;
	World->ServerTravel("/Game/Maps/Map_Lobby?listen");
}

/**
 * Handles the completion of destroying a session.
 *
 * This method is called when a session has been successfully destroyed or failed
 * to be destroyed. If the session destruction was successful, it logs a message
 * indicating the successful destruction and creates a new session.
 *
 * @param SessionName The name of the session that was destroyed.
 * @param Success True if the session was destroyed successfully, false otherwise.
 */
void USideScrollerGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Display, TEXT("Session, %s, Destroyed successfully."), *SessionName.ToString());
		CreateSession();
	}
}

/**
 * Function called when the find session search is complete.
 * @param Success Whether the find session search was successful or not.
 * @return None.
 */
void USideScrollerGameInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogTemp, Display, TEXT("Finding sessions is complete."));
	if (Success)
	{
		if (!GameSessionSearch.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("GameSessionSearch is not valid. Cant get find session results."));
			return;
		}
		TArray<FOnlineSessionSearchResult> SessionSearchResults = GameSessionSearch->SearchResults;
		if (SessionSearchResults.Num() > 0)
		{
			TArray<FServerData> ServerData;
			for (const FOnlineSessionSearchResult& SessionSearchResult : SessionSearchResults)
			{
				UE_LOG(LogTemp, Display, TEXT("Found session, %s with ping: %i ms."),
					*SessionSearchResult.GetSessionIdStr(), SessionSearchResult.PingInMs);
				FServerData Data;
				Data.MaxPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SessionSearchResult.Session.NumOpenPublicConnections;
				Data.HostUserName = SessionSearchResult.Session.OwningUserName;
				FString CustomServerName;
				if (SessionSearchResult.Session.SessionSettings.Get(SERVER_NAME_SESSION_KEY, CustomServerName))
				{
					if (CustomServerName.IsEmpty())
					{
						UE_LOG(LogTemp, Display, TEXT("Did not find custom server name, using default."));
						Data.ServerName = SessionSearchResult.GetSessionIdStr();
					}
					else
					{
						UE_LOG(LogTemp, Display, TEXT("Found custom server name, %s."), *CustomServerName);
						Data.ServerName = CustomServerName;
					}
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("Did not find custom server name, using default."));
					Data.ServerName = SessionSearchResult.GetSessionIdStr();
				}
				ServerData.Add(Data);
			}

			//FServerData Data1, Data2, Data3;

			//Data1.ServerName = "Test Server 1";
			//Data1.MaxPlayers = 3;
			//Data1.CurrentPlayers = Data1.MaxPlayers - 3;
			//Data1.HostUserName = "rando_name_1";
			//ServerData.Add(Data1);

			//Data2.ServerName = "Test Server 2";
			//Data2.MaxPlayers = 3;
			//Data2.CurrentPlayers = Data2.MaxPlayers - 3;
			//Data2.HostUserName = "some_other_player_name";
			//ServerData.Add(Data2);

			//Data3.ServerName = "Test Server 3";
			//Data3.MaxPlayers = 3;
			//Data3.CurrentPlayers = Data3.MaxPlayers - 3;
			//Data3.HostUserName = "Yo Momma";
			//ServerData.Add(Data3);

			Menu->SetServerList(ServerData);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("There were 0 game sessions found in the find session search."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game session search was not successful."));
	}
}

/**
 * Called when the join session process has completed.
 *
 * @param SessionName The name of the session to join.
 * @param Result The result of the join session process.
 */
void USideScrollerGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid())
	{
		FString Address;
		if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
		{
			UE_LOG(LogTemp, Display, TEXT("Could not get connect string."));
			return;
		}
		
		UEngine* Engine = GetEngine();
		if (!Engine) return;
		Engine->AddOnScreenDebugMessage(0,5,FColor::Green,
		FString::Printf(TEXT("Joining session %s"), *SessionName.ToString()));
	
		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (!PlayerController) return;
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SessionInterface is not valid. Cant get find join session."));
		return;
	}

}

/**
 * Creates a game session.
 *
 * @param None
 *
 * @return None
 */
void USideScrollerGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		if (Menu) NumPlayers = Menu->GetNumberOfPlayers();
		
		UE_LOG(LogTemp, Display,
			TEXT("Creating session, %s, for %i players."),
			*SESSION_NAME.ToString(),
			NumPlayers
		);
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL");
		SessionSettings.NumPublicConnections = NumPlayers;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		if (!DesiredServerName.IsEmpty())
		{
			SessionSettings.Set(SERVER_NAME_SESSION_KEY, DesiredServerName,
				EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		}
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

/**
 * Loads a saved game for the player profile.
 * If a saved game file exists, the method will load it.
 * If not, it will create a new instance of the player profile and save it.
 */
void USideScrollerGameInstance::LoadGame()
{
	UE_LOG(LogTemp, Display, TEXT("USideScrollerGameInstance::LoadGame - Trying to load a saved game"));
	// Try to load a saved game file (with name: <SaveGameSlotName>.sav) if exists
	PlayerProfile = Cast<USideScrollerSaveGame>(
		UGameplayStatics::LoadGameFromSlot(PlayerProfileSlot, 0)
	);

	// If file does not exist try create a new one
	if (PlayerProfile == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("USideScrollerGameInstance::LoadGame - No saved games found. Trying to save a new one.")
		);

		// Instantiate a new SaveGame object
		PlayerProfile = Cast<USideScrollerSaveGame>(
			UGameplayStatics::CreateSaveGameObject(USideScrollerSaveGame::StaticClass())
		);
		if (PlayerProfile == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("USideScrollerGameInstance::LoadGame - Not able to create a saved game.")
			);
			return;
		}
		
		PlayerProfile->PlayerName = TEXT("Default");
		PlayerProfile->VolumeLevel = 0.75;
		PlayerProfile->ResolutionIndex = 1;
		SaveGame();
	}
	else
	{
		
		UE_LOG(LogTemp, Display,
			TEXT("USideScrollerGameInstance::LoadGame - Saved game found. Loaded %s."), *PlayerProfile->GetPathName()
		);
	}
    
}

/**
 * Saves the current game progress.
 *
 * @param PlayerProfile - The save game object to be serialized and saved.
 * @param PlayerProfileSlot - The name of the save game slot.
 */
void USideScrollerGameInstance::SaveGame()
{
	UE_LOG(LogTemp, Display, TEXT("USideScrollerGameInstance::SaveGame - Saving game..."));
    
	// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
	if (UGameplayStatics::SaveGameToSlot(PlayerProfile, PlayerProfileSlot, 0))
	{
		UE_LOG(LogTemp, Display,
			TEXT("USideScrollerGameInstance::LogIfGameWasSavedOrNot - Game saved.")
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USideScrollerGameInstance::LogIfGameWasSavedOrNot - Game NOT saved.")
		);
	}
}

/**
 * @brief Get the player profile object.
 *
 * @return USideScrollerSaveGame* The player profile.
 */
USideScrollerSaveGame* USideScrollerGameInstance::GetPlayerProfile() const
{
	return PlayerProfile;
}
