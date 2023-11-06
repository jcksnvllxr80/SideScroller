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

const static FName SESSION_NAME = NAME_GameSession;
const static FName SERVER_NAME_SESSION_KEY = TEXT("CustomServerName");

USideScrollerGameInstance::USideScrollerGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!MainMenuBPClass.Class) return;
	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!InGameMenuBPClass.Class) return;
	InGameMenuClass = InGameMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameOverMenuBPClass(TEXT("/Game/MenuSystem/WBP_GameOverMenu"));
	if (!GameOverMenuBPClass.Class) return;
	GameOverMenuClass = GameOverMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> SelectCharacterMenuBPClass(
		TEXT("/Game/MenuSystem/WBP_SelectCharacterMenu")
	);
	if (!SelectCharacterMenuBPClass.Class) return;
	SelectCharacterMenuClass = SelectCharacterMenuBPClass.Class;

	LoadGame();
}

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

void USideScrollerGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	
	UE_LOG(LogTemp, Display, TEXT("USideScrollerGameInstance::LoadMainMenu - Loading MainMenu map."));
	PlayerController->ClientTravel("/Game/Maps/Map_MainMenu", ETravelType::TRAVEL_Absolute);
}

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

void USideScrollerGameInstance::SelectCharacterLoadMenu()
{
	if (SelectCharacterMenuClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found SelectCharacter Menu blueprint class %s."), *SelectCharacterMenuClass->GetName());
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

int USideScrollerGameInstance::GetNumPlayersToStartGame() const
{
	return NumPlayers;
}

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
		
		const auto ChosenCharacter = PlayerControllerChosenCharMap.find(PlayerFox->GetName());
		if (ChosenCharacter == PlayerControllerChosenCharMap.end())
		{
			UE_LOG(LogTemp, Display,
				TEXT("USideScrollerGameInstance::GetChosenCharacter - Cant find Player, %s, in the map."),
				*PlayerFox->GetName()
			)
			return nullptr;
		}
		return ChosenCharacter->second;
	}
	return nullptr;
}

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
	
	this->PlayerControllerChosenCharMap.insert({PlayerFox->GetName(), ChosenCharacter});
}

bool USideScrollerGameInstance::IsEveryPlayersCharacterChosen() const
{
	return (this->PlayerControllerChosenCharMap.size() >= GetNumPlayersToStartGame()) ? true : false;
}

void USideScrollerGameInstance::SetReadyToStartGame(bool bCond)
{
	this->bReadyToStartGame = bCond;
}

bool USideScrollerGameInstance::IsReadyToStartGame()
{
	return this->bReadyToStartGame;
}

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

void USideScrollerGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Display, TEXT("Session, %s, Destroyed successfully."), *SessionName.ToString());
		CreateSession();
	}
}

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
			//Data1.HostUserName = "Grabbir Bubi";
			//ServerData.Add(Data1);

			//Data2.ServerName = "Test Server 2";
			//Data2.MaxPlayers = 3;
			//Data2.CurrentPlayers = Data2.MaxPlayers - 3;
			//Data2.HostUserName = "Haida Salami";
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

USideScrollerSaveGame* USideScrollerGameInstance::GetPlayerProfile() const
{
	return PlayerProfile;
}
