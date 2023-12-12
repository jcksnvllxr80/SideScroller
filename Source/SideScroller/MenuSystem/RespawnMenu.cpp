// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnMenu.h"

#include "Components/Button.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

bool URespawnMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (RespawnButton)
	{
		RespawnButton->OnClicked.AddDynamic(this, &URespawnMenu::Respawn);

		RespawnButton->SetIsEnabled(false);
		RunRespawnTimer();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::Initialize - INIT FAILED! Cant find the Respawn button during init.")
		);
		return false;
	}
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &URespawnMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::Initialize - INIT FAILED! Cant find the MainMenu button during init.")
		);
		return false;
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &URespawnMenu::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::Initialize - INIT FAILED! Cant find the Quit button during init.")
		);
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("URespawnMenu::Initialize - Respawn Menu Init complete!"));
	return true;
}

void URespawnMenu::BackToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("URespawnMenu::BackToMainMenu - OPEN MAIN MENU FAILED! Cant find the Menu Interface.")
		);
		return;
	}
}

void URespawnMenu::Respawn()
{
	if (const UWorld* World = GetWorld())
	{
		if (const APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (APawn* PawnToRespawn = PlayerController->GetPawn())
			{
				APC_PlayerFox* PlayerPawnToRespawn = Cast<APC_PlayerFox>(PawnToRespawn);
				if (PlayerPawnToRespawn != nullptr)
				{
					UE_LOG(LogTemp, Display,
						TEXT("URespawnMenu::Respawn - Player, %s, is respawning."),
						*PlayerPawnToRespawn->GetPlayerName().ToString()
					);
					PlayerPawnToRespawn->ReviveAtCheckpoint();
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("URespawnMenu::Respawn - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URespawnMenu::Respawn - Cant find World."));
		return;
	}
}

void URespawnMenu::QuitGame()
{
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			const FString QuitCommand = TEXT("quit");
			PlayerController->ConsoleCommand(QuitCommand);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("URespawnMenu::QuitGame - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URespawnMenu::QuitGame - Cant find World."));
		return;
	}
}

void URespawnMenu::RunRespawnTimer()
{
	// TODO: show respawn countdown on respawn menu
	
	GetWorld()->GetTimerManager().SetTimer(
		this->RespawnDelayTimerHandle,
		this,
		&URespawnMenu::EnableRespawnButton,
		RespawnDelayTime,
		false
	);
}

void URespawnMenu::EnableRespawnButton() const
{
	RespawnButton->SetIsEnabled(true);
}
