// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverMenu.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool UGameOverMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &UGameOverMenu::RestartGame);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::Initialize - INIT FAILED! Cant find the RestartGame button during init.")
		);
		return false;
	}
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UGameOverMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::Initialize - INIT FAILED! Cant find the MainMenu button during init.")
		);
		return false;
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UGameOverMenu::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::Initialize - INIT FAILED! Cant find the Quit button during init.")
		);
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("UGameOverMenu::Initialize - GameOver Menu Init complete!"));
	return true;
}

void UGameOverMenu::BackToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGameOverMenu::BackToMainMenu - OPEN MAIN MENU FAILED! Cant find the Menu Interface.")
		);
		return;
	}
}

void UGameOverMenu::RestartGame()
{
	UE_LOG(LogTemp, Display, TEXT("UGameOverMenu::RestartGame - Leaving GameOver Menu to start game..."));
	UWorld* World = GetWorld();
	if (!World) return;
	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Map_Level1?listen");
}

void UGameOverMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			const FString QuitCommand = TEXT("quit");
			PlayerController->ConsoleCommand(QuitCommand);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UGameOverMenu::QuitGame - Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGameOverMenu::QuitGame - Cant find World."));
		return;
	}
}
