// Fill out your copyright notice in the Description page of Project Settings.


#include "SideScrollerGameInstance.h"

// #include <ThirdParty/CryptoPP/5.6.5/include/argnames.h>

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
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
	PlayerController->ClientTravel("/Game/Maps/Map_MainMenu", ETravelType::TRAVEL_Absolute);
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
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (!ExistingSession)
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
		UE_LOG(LogTemp, Display, TEXT("Found In Game Menu blueprint class %s."), *InGameMenuClass->GetName());
		UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
		if (InGameMenu)
		{
			InGameMenu->Setup();
			InGameMenu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMenuWidget Menu from main menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the Main Menu blueprint class."));
		return;
	}
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
	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void USideScrollerGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Display, TEXT("Session, %s, Destroyed successfully."));
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
						UE_LOG(LogTemp, Warning, TEXT("Did not find custom server name, using default."));
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
					UE_LOG(LogTemp, Warning, TEXT("Did not find custom server name, using default."));
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
			UE_LOG(LogTemp, Warning, TEXT("There were 0 game sessions found in the find session search."));
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
			UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
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
		UE_LOG(LogTemp, Display, TEXT("Creating session, %s."), *SESSION_NAME.ToString());
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL");
		SessionSettings.NumPublicConnections = 3;
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
