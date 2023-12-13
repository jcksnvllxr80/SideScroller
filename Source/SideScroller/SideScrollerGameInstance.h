// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/MenuInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "GameModes/LobbyGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SidescrollerGameInstance.generated.h"

/**
 * @class USideScrollerSaveGame
 * @brief This class represents a save game for the SideScroller game.
 *
 * The USideScrollerSaveGame class inherits from the USaveGame class, and it contains the saved data for a player's
 * progress in the game.
 */
class USideScrollerSaveGame;
/**
 * @class APC_PlayerFox
 * @brief The APC_PlayerFox class represents the player character in the game.
 *
 * This class is responsible for managing player-related functionality, such as tracking
 * points, lives, and interactable objects. It also handles interactions with other players
 * and manages animations.
 */
class APC_PlayerFox;

/**
 * @struct FServerData
 * @brief Represents server data information.
 */
USTRUCT()
struct FServerData
{
	GENERATED_BODY()
	/**
	 * @brief Represents the name of the server.
	 *
	 * This variable holds the name of the server for identification purposes.
	 * It is of type FString, which is a string datatype in Unreal Engine.
	 *
	 * Use this variable to store the name of the server where necessary in your code.
	 *
	 * @see FString
	 */
	FString ServerName;
	/**
	 * @brief The number of current players in the game.
	 *
	 * This variable represents the current number of players playing the game.
	 * It is of type `uint16`, which is an unsigned integer with a range from 0 to 65535.
	 *
	 * Usage:
	 * - To access the number of current players, simply refer to the variable `CurrentPlayers`.
	 * - The value of `CurrentPlayers` should be updated according to the actual number of players in the game.
	 */
	uint16 CurrentPlayers;
	/**
	 * @brief Maximum number of players allowed in the game.
	 *
	 * This variable represents the maximum number of players that can participate in the game.
	 * It is of type 'uint16' which is an unsigned integer capable of storing values from 0 to 65535.
	 */
	uint16 MaxPlayers;
	/**
	 * @brief The username of the host.
	 *
	 * This variable stores the username of the host.
	 *
	 * Example Usage:
	 * @code
	 * FString HostUserName;
	 * HostUserName = "JohnDoe";
	 * @endcode
	 */
	FString HostUserName;
};

/**
 * \class USideScrollerGameInstance
 *
 * \brief A class that represents the game instance for the side scroller game.
 *
 * This class is responsible for managing the game instance and implementing various game-related functions.
 * It handles loading and initializing the main menu, refreshing the server list, hosting and joining game sessions,
 * managing player profiles, saving and loading game data, and more.
 */
UCLASS()
class SIDESCROLLER_API USideScrollerGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	/**
	 * Constructor for the SideScrollerGameInstance class.
	 * Initializes the class by finding and assigning the necessary widget classes for the main menu, in-game menu,
	 * game over menu, and select character menu. It also loads the saved game.
	 *
	 * @param ObjectInitializer The object initializer from FObjectInitializer.
	 */
	USideScrollerGameInstance(const FObjectInitializer & ObjectInitializer);

	/**
	 * Initializes the game instance.
	 *
	 * This method initializes the game instance by getting the online subsystem, checking for the session interface,
	 * and setting up the delegates for different session events.
	 *
	 * @see IOnlineSubsystem::Get()
	 * @see IOnlineSubsystem::GetSubsystemName()
	 * @see IOnlineSubsystem::GetSessionInterface()
	 * @see SessionInterface->OnCreateSessionCompleteDelegates
	 * @see SessionInterface->OnDestroySessionCompleteDelegates
	 * @see SessionInterface->OnFindSessionsCompleteDelegates
	 * @see SessionInterface->OnJoinSessionCompleteDelegates
	 */
	virtual void Init();

	/**
	 * Loads the main menu level. This function is a BlueprintCallable and can be called from Blueprints.
	 *
	 * @param void
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;

	/**
	 * @brief Refreshes the server list.
	 *
	 * This function is responsible for refreshing the server list. It searches for available game sessions
	 * using the SessionInterface object and populates the GameSessionSearch object with the results.
	 * The maximum number of search results can be set using the MaxSearchResults property of the GameSessionSearch
	 * object. The search can be customized using the QuerySettings property of the GameSessionSearch object.
	 *
	 * @note This function assumes that the SessionInterface object is valid. If it is not valid, an error message will
	 * be logged and the function will exit early.
	 *
	 * @note This function assumes that the GameSessionSearch object is valid. If it is not valid, an error message
	 * will be logged and the function will exit early.
	 *
	 * @see FOnlineSessionSearch, FOnlineSessionSearchSettings
	 */
	UFUNCTION(BlueprintCallable)
	void RefreshServerList() override;

	/**
	 * @brief This function is used to host a game session.
	 *
	 * @param ServerName The name of the server for the game session.
	 *
	 * This function is executed when the host command is called. It is used to start hosting a game
	 * session with the specified server name. If there is no valid session interface, an error message
	 * is logged and the function exits early. If there is an existing session with the same name,
	 * it is destroyed before creating a new session. Otherwise, a new session is created.
	 */
	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	/**
	 * @brief Function to join a game with the given IP address.
	 *
	 * This function is used to join a game session with the provided IP address.
	 * It first checks if the engine is valid and if not, it returns without doing anything.
	 *
	 * It then adds an on-screen debug message indicating that it is joining the IP address.
	 *
	 * Next, it gets the first local player controller and checks if it is valid.
	 * If not, it returns without doing anything.
	 *
	 * Finally, it calls the ClientTravel function of the player controller with the provided IP address
	 * and ETravelType::TRAVEL_Absolute as the travel type.
	 *
	 * @param IpAddress The IP address of the game session to join.
	 */
	UFUNCTION(Exec)
	void JoinIP(FString& IpAddress) override;

	/**
	 * @brief Joins a game session at the specified index.
	 *
	 * This function joins a game session at a specific index. It checks if the session interface and game session
	 * search is valid before attempting to join the session. If the session interface or game session search is not
	 * valid, an error message is logged and the function returns.
	 *
	 * @param Index The index of the game session to join.
	 * @return void
	 */
	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	/** Loads the menu by creating a widget instance of the Main Menu blueprint class.
	 *
	 * - If the MainMenuClass is valid, it logs a message indicating the class is found.
	 * - Creates an instance of MainMenu widget.
	 * - Sets up the MainMenu widget.
	 * - Sets the MenuInterface of the MainMenu widget to be the current game instance.
	 * - If MainMenu widget fails to create, it logs an error message and returns.
	 * - If MainMenuClass is not found, it logs an error message and returns.
	 *
	 * @see MainMenuClass
	 */
	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	/**
	 * Loads the in-game menu.
	 *
	 * This method creates and sets up an instance of the in-game menu widget if the in-game menu blueprint
	 * class is found.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	/**
	 * RespawnLoadMenu method is responsible for displaying the Respawn Menu widget.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void RespawnLoadMenu();

	/**
	 * SelectCharacterLoadMenu
	 *
	 * Opens the Select Character load menu if the blueprint class is available.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void SelectCharacterLoadMenu();

	/**
	 * Loads the game over menu.
	 *
	 * If the GameOverMenuClass variable is set, this method will create an instance of the GameOverMenu blueprint class,
	 * set it up, and set the current menu interface to the created menu. Otherwise, it will log an error message.
	 *
	 * @note This method is BlueprintCallable, meaning it can be called from Blueprint scripts.
	 */
	UFUNCTION(BlueprintCallable)
	void GameOverLoadMenu();

	/**
	 * Loads the Game Over Menu.
	 *
	 * This function is called to load the Game Over Menu. It checks if the current game mode is a level game mode,
	 * and if so, it calls the TravelToGameOverMenu function of the level game mode.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void LoadGameOverMenu();

	/**
	 * @brief Get the number of players required to start the game.
	 *
	 * @return The number of players required to start the game.
	 */
	UFUNCTION(BlueprintCallable)
	int GetNumPlayersToStartGame() const;

	/**
	 * Retrieves the chosen character class associated with the given player controller.
	 * Returns the class of the chosen character.
	 *
	 * @param PlayerController The player controller whose chosen character class will be retrieved.
	 * @return The class of the chosen character, or nullptr if no character has been chosen or the player controller
	 * is not associated with a character.
	 */
	UFUNCTION(BlueprintCallable)
	TSubclassOf<APC_PlayerFox> GetChosenCharacter(APlayerController* PlayerController);

	/**
	 * Sets the chosen character for the specified player controller.
	 *
	 * @param PlayerController The player controller for which to set the chosen character.
	 * @param ChosenCharacter The subclass of APC_PlayerFox to set as the chosen character.
	 */
	UFUNCTION(BlueprintCallable)
	void SetChosenCharacter(APlayerController* PlayerController, TSubclassOf<APC_PlayerFox> ChosenCharacter);

	/**
	 * Check whether every player has chosen a character.
	 *
	 * @return True if every player has chosen a character, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsEveryPlayersCharacterChosen() const;

	/**
	 * @brief Sets the ready-to-start game condition.
	 *
	 * This method updates the ready-to-start game condition of the `USideScrollerGameInstance` instance.
	 * The condition determines whether the game is ready to start or not.
	 *
	 * @param bCond The new value of the ready-to-start game condition.
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void SetReadyToStartGame(bool bCond);

	/**
	 * @brief Checks if the game is ready to start.
	 *
	 * @return True if the game is ready to start, false otherwise.
	 *
	 * @param None.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsReadyToStartGame();

	/**
	 * \brief Retrieves the player profile.
	 *
	 * \return The player's profile as an instance of USideScrollerSaveGame.
	 */
	UFUNCTION(BlueprintCallable)
	USideScrollerSaveGame* GetPlayerProfile() const;

	/**
	 * Saves the game.
	 *
	 * This function serializes and saves the player's profile to a save game slot.
	 *
	 * @param PlayerProfile - The player's profile to save.
	 * @param PlayerProfileSlot - The name of the save game slot.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	/**
	 * @brief Retrieves the current level.
	 *
	 * @return The current level.
	 */
	UFUNCTION(BlueprintCallable)
	int GetCurrentLevel() const;

	/**
	 * @brief Increments the current level by 1.
	 *
	 * This method is used to increment the current level of the game instance by 1. It updates the
	 * `CurrentLevel` variable and logs a warning message with the old and new values of `CurrentLevel`.
	 */
	UFUNCTION(BlueprintCallable)
	void IncrementCurrentLevel();
	
private:
	/**
	 * @brief The MainMenuClass variable.
	 *
	 * MainMenuClass represents the class of the main menu user widget.
	 */
	TSubclassOf<class UUserWidget> MainMenuClass = nullptr;
	
	/**
	 * @brief This variable represents the User Widget class for the in-game menu.
	 *
	 * The InGameMenuClass variable is of type TSubclassOf<class UUserWidget> and is used to store the class
	 * that represents the in-game menu User Widget. The User Widget is responsible for displaying the
	 * in-game menu UI elements.
	 *
	 * Setting this variable to nullptr means that there is no specific User Widget class assigned to the
	 * in-game menu yet.
	 *
	 * @note The actual User Widget class assigned to this variable should be derived from the UUserWidget
	 *       base class.
	 *
	 * @see UUserWidget
	 */
	TSubclassOf<class UUserWidget> InGameMenuClass = nullptr;
	
	/**
	 * @brief The class representing the Select Character Menu.
	 *
	 * This class is responsible for managing the Select Character Menu user interface.
	 * It extends the UUserWidget class which is a base class for all user widgets.
	 *
	 * @see UUserWidget
	 */
	TSubclassOf<class UUserWidget> SelectCharacterMenuClass = nullptr;
	
	/**
	 *
	 */
	TSubclassOf<class UUserWidget> SettingsMenuClass = nullptr;
	
	/**
	 * @brief Class variable representing the game over menu widget.
	 *
	 * This variable holds a reference to the UE4 `TSubclassOf<class UUserWidget>` class which represents the game
	 * over menu widget. It is initialized with a `nullptr` value to indicate that it has not been assigned yet.
	 *
	 * @note To create an instance of the game over menu widget, use `CreateWidget` or `CreateWidgetInstance` function
	 * and pass this variable as the template parameter.
	 *
	 * @see CreateWidget
	 * @see CreateWidgetInstance
	 */
	TSubclassOf<class UUserWidget> GameOverMenuClass = nullptr;

	/**
	 * The RespawnMenuClass variable is a TSubclassOf<UUserWidget> type variable used to hold the class of the
	 * respawn menu widget. It is assigned with nullptr by default.
	 *
	 * @see UUserWidget
	 */
	TSubclassOf<class UUserWidget>  RespawnMenuClass = nullptr;
	
	/**
	 * @class UMainMenu
	 * @brief A class representing a main menu in a software application.
	 *
	 * This class is responsible for managing the main menu interface of the application.
	 * It provides functionalities for creating, displaying, and interacting with the menu.
	 * The UMainMenu class is used as a pointer in the variable Menu.
	 *
	 * Usage:
	 *   UMainMenu* Menu;
	 *   // Instantiate a UMainMenu object and assign it to the Menu variable
	 *   Menu = new UMainMenu();
	 *
	 *   // Use the Menu object to perform operations related to the main menu
	 *   Menu->Create();
	 *   Menu->Display();
	 *   Menu->Interact();
	 *   //...
	 *
	 *   // Deallocate the memory occupied by the Menu object
	 *   delete Menu;
	 *
	 * @see UMainMenu::Create
	 * @see UMainMenu::Display
	 * @see UMainMenu::Interact
	 */
	class UMainMenu* Menu;
	
	/**
	 * @brief Declaration of a smart pointer to an online session object.
	 *
	 * This variable is used to manage the online session interface. It is declared as a smart pointer
	 * type, allowing for automatic memory management. The pointer points to an object of type
	 * IOnlineSession, which provides functionality for managing online sessions.
	 *
	 * @see IOnlineSession
	 */
	IOnlineSessionPtr SessionInterface;
	
	/**
	 * @brief The GameSessionSearch variable is a shared pointer to an instance of the FOnlineSessionSearch class.
	 *
	 * The FOnlineSessionSearch class is used to perform searches for online game sessions. The GameSessionSearch
	 * variable acts as a handle to manage and access the instance of the FOnlineSessionSearch class.
	 *
	 * When creating a new search for game sessions, use the GameSessionSearch variable to store the instance of the
	 * FOnlineSessionSearch class. This variable can be used to perform various operations such as setting
	 * search criteria, initiating the search, and accessing the search results.
	 *
	 * Usage example:
	 * @code
	 * // Create a new search for game sessions
	 * GameSessionSearch = MakeShareable(new FOnlineSessionSearch());
	 *
	 * // Set search criteria, e.g., the required game mode
	 * GameSessionSearch->QuerySettings.Set("GameMode", "Deathmatch");
	 *
	 * // Initiate the search for game sessions
	 * if (GameSessionSearch->StartSearch())
	 * {
	 *     // Search started successfully, wait for the search results
	 *     // ...
	 * }
	 * else
	 * {
	 *     // Failed to start the search, handle the error
	 *     // ...
	 * }
	 * @endcode
	 */
	TSharedPtr<class FOnlineSessionSearch> GameSessionSearch;
	
	/**
	 * @brief The number of players in the game.
	 *
	 * This variable stores the number of players in the game. It is an
	 * integer type and should be initialized with the desired number
	 * of players before starting the game.
	 *
	 * @note The value of this variable should not be changed during the
	 *       course of the game.
	 */
	int NumPlayers = 1;
	
	/**
	 * @brief A boolean variable indicating whether the game is ready to start or not.
	 *
	 * This variable is used to track the readiness of the game to start.
	 * It is initially set to false and should be set to true when all the necessary preparations for the
	 * game are complete.
	 *
	 * @see StartGame()
	 *
	 * @remark This variable should be checked before attempting to start the game to ensure that all the
	 * required components are ready.
	 */
	bool bReadyToStartGame = false;
	
	/**
	 * Callback function called when a game session is complete.
	 *
	 * This function is called when a game session has been completed, indicating whether the session was
	 * successful or not.
	 *
	 * @param SessionName The name of the game session.
	 * @param Success     A boolean value indicating whether the session was successful (true) or not (false).
	 *
	 * @see USideScrollerGameInstance, GetEngine(), GetWorld(), UEngine, UWorld
	 */
	void OnGameSessionComplete(FName SessionName, bool Success);
	/**
	 * Called when the network session destruction is complete.
	 *
	 * @param SessionName    The name of the destroyed session.
	 * @param Success        Indicates whether the session destruction was successful or not.
	 */
	void OnDestroySessionComplete(FName SessionName, bool Success);
	/**
	 * Callback function for completing the find sessions operation.
	 *
	 * This function is called when the find sessions operation is complete.
	 *
	 * @param Success Boolean value indicating whether the find sessions operation was successful.
	 *
	 * @note If Success is true, the function retrieves the search results and updates the server list in the menu.
	 * If Success is false, a warning message is logged.
	 * If the GameSessionSearch object is not valid, an error message is logged and the function returns.
	 * If no game sessions were found in the search, a message is logged.
	 * Each found session is processed to obtain server data such as the maximum number of players, current number of
	 * players, host username, and server name. If a custom server name is found, it is used as the server name;
	 * otherwise, the session ID is used.
	 */
	void OnFindSessionsComplete(bool Success);
	/**
	 * @brief Callback function called when the join session operation completes.
	 *
	 * This function is called after attempting to join a session, and is used to handle the result
	 * of the operation. It takes in the name of the session being joined and the result of the operation.
	 *
	 * If the join operation is successful, it retrieves the connect string for the session and displays
	 * a debug message indicating that the session is being joined. It then uses the first local player
	 * controller to travel to the specified address to join the session.
	 *
	 * If the join operation fails or the session interface is not valid, an error message is logged and
	 * the function returns.
	 *
	 * @param SessionName The name of the session being joined.
	 * @param Result The result of the join session operation.
	 */
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	/**
	 * @brief PlayerControllerChosenCharMap
	 *
	 * This variable represents a map that associates a FString key with a TSubclassOf<APC_PlayerFox> value.
	 * It is used to store the chosen character for each player controller in the game.
	 *
	 * @details
	 * The map allows multiple player controllers to choose a character from a list of available character classes.
	 * The FString key represents a unique identifier for each player controller.
	 * The TSubclassOf<APC_PlayerFox> value represents the chosen character class for each player controller.
	 *
	 * @note
	 * It is important to note that the chosen character class must be a subclass of APC_PlayerFox, which is an actor
	 * that represents the player controller's character in the game world.
	 *
	 * @see
	 * APFC_PlayerFox
	 */
	std::map<FString, TSubclassOf<APC_PlayerFox>> PlayerControllerChosenCharMap;
	/**
	 * @brief The DesiredServerName variable stores the name of the desired server.
	 *
	 * The DesiredServerName variable is of type FString and is used to store the name
	 * of the desired server. This variable is typically used in networking or server
	 * related functions or classes.
	 */
	FString DesiredServerName;
	/**
	 * \brief Creates a session for the game instance.
	 *
	 * This method creates a game session with the specified settings using the online session interface. The session
	 * is created with the following settings:
	 * - The number of public connections is set to the value obtained from the menu.
	 * - LAN match is enabled if the online subsystem name is "NULL".
	 * - The session is set to be advertised and uses presence.
	 * - Lobbies are used if available.
	 * - The server name is set if a desired server name is provided.
	 *
	 * \param None.
	 *
	 * \return None.
	 *
	 */
	void CreateSession();

	/**
	 * @brief CurrentLevel is an integer variable representing the current level in a game.
	 *
	 * It is annotated with UPROPERTY() macro to make it accessible and editable through the Unreal Engine editor.
	 *
	 * The default value of CurrentLevel is 1, indicating the initial level of the game. It can be modified to track
	 * the current progress or to set a specific level.
	 */
	UPROPERTY()
	int CurrentLevel = 1;

	/**
	 * \brief Loads a saved game.
	 *
	 * This function attempts to load a saved game from a specified slot. If a saved game file
	 * exists in the slot, it is loaded into the PlayerProfile variable. If the saved game file
	 * does not exist, a new save game object is created and saved to the slot.
	 *
	 * \param None.
	 *
	 * \return None.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	/**
	 * The PlayerProfile variable is used to store an instance of the USideScrollerSaveGame class.
	 * This class is responsible for managing the saved game data for the player.
	 *
	 * Example Usage:
	 *   // Create a new player profile
	 *   USideScrollerSaveGame* NewProfile = NewObject<USideScrollerSaveGame>();
	 *
	 *   // Set the PlayerProfile variable to the new profile
	 *   PlayerProfile = NewProfile;
	 *
	 * Note:
	 *   - This variable is declared as a UPROPERTY() to ensure proper memory management and garbage collection.
	 *   - It is recommended to check if the PlayerProfile is valid before accessing its data.
	 *
	 * @see USideScrollerSaveGame
	 */
	UPROPERTY()
	USideScrollerSaveGame* PlayerProfile;

	/**
	 * @brief A string variable representing the name of the player profile slot.
	 *
	 * This variable is used to define the slot where the player profile data will be stored.
	 * The value of this variable is set to "SideScrollerPlayerProfile" by default.
	 *
	 * Usage:
	 * FString MyPlayerProfileSlot = PlayerProfileSlot;
	 *
	 * @see FString
	 */
	UPROPERTY()
	FString PlayerProfileSlot = "SideScrollerPlayerProfile";
};
