// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "SideScroller/SideScrollerGameInstance.h"

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

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	SetNumPlayersToStart();
	LogPlayerCount();
	EnablePlayerGameModeInput(NewPlayer);
	
	if(NumberOfPlayers >= MinPlayersToStartGame)
	{
		bNumPlayersRequirementFulfilled = true;
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

bool ALobbyGameMode::IsNumPlayersRequirementFulfilled() const
{
	return bNumPlayersRequirementFulfilled;
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
	LogPlayerCount();
}

void ALobbyGameMode::SetNumPlayersToStart()
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
