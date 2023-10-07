// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	UE_LOG(LogTemp, Warning, TEXT("There are %i players in the lobby."), NumberOfPlayers);
	if(NumberOfPlayers >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Leaving lobby to start game..."), NumberOfPlayers);
		UWorld* World = GetWorld();
		if (!World) return;
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
	UE_LOG(LogTemp, Warning, TEXT("There are %i players in the lobby."), NumberOfPlayers);
}
