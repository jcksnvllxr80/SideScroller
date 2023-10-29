// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterMenu.h"
#include "Components/Button.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Controllers/GameModePlayerController.h"

void USelectCharacterMenu::PinkPlayerSelect()
{
	SelectPlayer(PinkPlayerBP, "Pink");
}

void USelectCharacterMenu::OrangePlayerSelect()
{
	SelectPlayer(OrangePlayerBP, "Orange");
}

void USelectCharacterMenu::YellowPlayerSelect()
{
	SelectPlayer(YellowPlayerBP, "Yellow");
}

void USelectCharacterMenu::GreenPlayerSelect()
{
	SelectPlayer(GreenPlayerBP, "Green");
}

void USelectCharacterMenu::BluePlayerSelect()
{
	SelectPlayer(BluePlayerBP, "Blue");
}

void USelectCharacterMenu::BlackPlayerSelect()
{
	SelectPlayer(BlackPlayerBP, "Black");
}

bool USelectCharacterMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;

	if (PinkPlayerButton)
	{
		PinkPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::PinkPlayerSelect);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the PinkPlayer Button during init."));
		return false;
	}

	if (OrangePlayerButton)
	{
		OrangePlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::OrangePlayerSelect);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the OrangePlayer Button during init."));
		return false;
	}

	if (YellowPlayerButton)
	{
		YellowPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::YellowPlayerSelect);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the YellowPlayer Button during init."));
		return false;
	}

	if (GreenPlayerButton)
	{
		GreenPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::GreenPlayerSelect);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the GreenPlayer Button during init."));
		return false;
	}

	if (BluePlayerButton)
	{
		BluePlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BluePlayerSelect);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the BluePlayer Button during init."));
		return false;
	}

	if (BlackPlayerButton)
	{
		BlackPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BlackPlayerSelect);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the BlackPlayer Button during init."));
		return false;
	}
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BackToGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Cancel button during init."));
		return false;
	}
	
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Exit button during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Select Character Menu Init complete!"));
	return true;
}

void USelectCharacterMenu::BackToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OPEN MAIN MENU FAILED! Cant find the Menu Interface."));
		return;
	}
}

void USelectCharacterMenu::BackToGame()
{
	OnLevelRemovedFromWorld();
	GetWorld()->GetFirstPlayerController()->SetPause(false);
}

void USelectCharacterMenu::SelectPlayer(const TSubclassOf<APC_PlayerFox> PlayerBP, const FString& PlayerColorStr)
{
	UE_LOG(LogTemp, Display,
		TEXT("USelectCharacterMenu::SelectPlayer - Player Selected the %s player character."),
		*PlayerColorStr
	);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("USelectCharacterMenu::SelectPlayer - Select %s Character Failed! No PlayerController."),
			*PlayerColorStr
		);
	}

	AGameModePlayerController* GameModePlayerController = Cast<AGameModePlayerController>(PlayerController);
	if (GameModePlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USelectCharacterMenu::SelectPlayer - Select %s character failed! No GameModePlayerController."),
			*PlayerColorStr
		);
		return;
	}

	GameModePlayerController->SpawnPlayer(PlayerBP, PlayerColorStr, PlayerController);
	BackToGame();
	GameModePlayerController->CheckGameStartReqs();
	GameModePlayerController->TravelToLevel();
}
