// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		this->CharacterSelectDelayTimerHandle,
		this,
		&ALobbyGameState::PlayerSpawnCharacterSelect,
		this->CharacterSelectDelayTimer,
		false
	);
}

void ALobbyGameState::PlayerSpawnCharacterSelect()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALobbyGameState::PlayerSpawnCharacterSelect - Cant find GameInstance!")
		);
		return;  // early return
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController != nullptr)
	{
		// if the player hasn't selected and stored a character yet, open the character select window
		if (GameInstance->GetChosenCharacter(PlayerController) == nullptr)
		{
			OpenSelectCharacterMenu();
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("ALobbyGameState::PlayerSpawnCharacterSelect - Player has previously chosen a character!")
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALobbyGameState::PlayerSpawnCharacterSelect - Cant find PlayerController!")
		);
	}
}

void ALobbyGameState::OpenSelectCharacterMenu()
{
	const ALobbyGameState* LobbyGameState = dynamic_cast<ALobbyGameState*>(GetWorld()->GetGameState());
	if (LobbyGameState != nullptr)
	{
		USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
		if (GameInstance != nullptr) {
			GameInstance->SelectCharacterLoadMenu();
		} else {
			UE_LOG(LogTemp, Warning,
				TEXT("ALobbyGameState::OpenSelectCharacterMenu - Cant find GameInstance!")
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALobbyGameState::OpenSelectCharacterMenu - Cant find LobbyGameState!")
		);
	}
}
