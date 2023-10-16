// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "SideScroller/SideScrollerGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	GetNumPlayersToStart();
	LogPlayerCount();
	EnablePlayerGameModeInput(NewPlayer);
	
	if(NumberOfPlayers >= MinPlayersToStartGame)
	{
		UE_LOG(LogTemp, Display, TEXT("Leaving lobby to start game..."));
		UWorld* World = GetWorld();
		if (!World) return;
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/Map_Level1?listen");
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
	LogPlayerCount();
}

void ALobbyGameMode::GetNumPlayersToStart()
{
	const USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr)
	{
		this->MinPlayersToStartGame = GameInstance->GetNumPlayersToStartGame();
	}
}

void ALobbyGameMode::LogPlayerCount() const
{
	UE_LOG(LogTemp, Display,
		   TEXT("There are %i players in the lobby (out of %i needed)."),
		   NumberOfPlayers,
		   MinPlayersToStartGame
	);
}
