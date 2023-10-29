// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Controllers/GameModePlayerController.h"

void ALevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		this->SpawnPlayerChosenCharDelayTimerHandle,
		this,
		&ALevelGameMode::SpawnPlayerChosenCharacters,
		SpawnPlayerChosenCharDelayTimer,
		false
	);
}

bool ALevelGameMode::LocateChosenCharacter(FConstPlayerControllerIterator Iter)
{
	APlayerController* PlayerController = Cast<APlayerController>(*Iter);
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find PlayerController.")
		)
		return true;  // start loop over cant find this player controller
	}
		
	UE_LOG(LogTemp, Display,
	       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Iterating over Players. Currently on %s."),
	       *PlayerController->GetName()	
	)
		
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find GameInstance.")
		)
		return false;  // dont go any further, cant find game instance
	}

	AGameModePlayerController* GameModePlayerController = Cast<AGameModePlayerController>(PlayerController);
	if (GameModePlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - PlayerController is not a \"GameMode\" PC.")
		)
		return true;  // start loop over this player controller is not a GameMode player controller; next!
	}
		
	const TSubclassOf<APC_PlayerFox> ChosenCharacterBP = GameInstance->GetChosenCharacter(PlayerController);
	if (ChosenCharacterBP != nullptr)
	{
		GameModePlayerController->SpawnPlayer(ChosenCharacterBP, "", PlayerController);
		UE_LOG(LogTemp, Display,
		       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Spawning saved chosen player character.")
		)
	}
	else
	{
		if (DefaultCharacterBP != nullptr)
		{
			GameModePlayerController->SpawnPlayer(DefaultCharacterBP, "", PlayerController);
			UE_LOG(LogTemp, Display,
			       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Spawning default player character.")
			)
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - default player character is null - no spawn.")
			)
		}
	}
	return true;
}

void ALevelGameMode::SpawnPlayerChosenCharacters()
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find World.")
		)
		return;  // dont go any further, cant find world
	}

	for (FConstPlayerControllerIterator Iter = World->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		if (!LocateChosenCharacter(Iter)) return;
	}
}
