// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
#include "SideScroller/GameStates/Level1GameState.h"

void ALevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	SpawnPlayerChosenCharacters();
}

void ALevelGameMode::SpawnPlayerChosenCharacters()
{
	const UWorld* World = GetWorld();
	if (World != nullptr)
	{
		for (FConstPlayerControllerIterator Iter = World->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			APlayerController* PlayerController = Cast<APlayerController>(*Iter);
			if (PlayerController != nullptr)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Iterating over Players. Currently on %s."),
					*PlayerController->GetName()	
				)
				
				USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetGameInstance());
				if (GameInstance != nullptr)
				{
					AGameModePlayerController* GameModePlayerController = Cast<AGameModePlayerController>(PlayerController);
					if (GameModePlayerController != nullptr)
					{
						const TSubclassOf<APC_PlayerFox> ChosenCharacterBP = GameInstance->GetChosenCharacter(
							PlayerController
						);
						
						if (ChosenCharacterBP != nullptr)
						{
							GameModePlayerController->SpawnPlayer(ChosenCharacterBP, "", PlayerController);
							UE_LOG(LogTemp, Warning,
								TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Spawning saved chosen player char.")
							)
						}
						else
						{
							if (DefaultCharacterBP != nullptr)
							{
								GameModePlayerController->SpawnPlayer(DefaultCharacterBP, "", PlayerController);
								UE_LOG(LogTemp, Warning,
									TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Spawning default player char.")
								)
							}
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning,
						TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find GameInstance.")
					)
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning,
					TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find PlayerController.")
				)
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALevelGameMode::SpawnPlayerChosenCharacters - Cant find World.")
		)
	}
}
