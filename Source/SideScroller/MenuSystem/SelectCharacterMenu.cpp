// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterMenu.h"
#include "Components/Button.h"

void USelectCharacterMenu::PinkPlayerSelect()
{
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::PinkPlayerSelect - Player Selected the Pink Player."));
}

void USelectCharacterMenu::OrangePlayerSelect()
{
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::OrangePlayerSelect - Player Selected the Orange Player."));
}

void USelectCharacterMenu::YellowPlayerSelect()
{
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::YellowPlayerSelect - Player Selected the Yellow Player."));
}

void USelectCharacterMenu::GreenPlayerSelect()
{
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::GreenPlayerSelect - Player Selected the Green Player."));
}

void USelectCharacterMenu::BluePlayerSelect()
{
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::BluePlayerSelect - Player Selected the Blue Player."));
}

void USelectCharacterMenu::BlackPlayerSelect()
{
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::BlackPlayerSelect - Player Selected the Black Player.."));
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

	UE_LOG(LogTemp, Display, TEXT("In Game Menu Init complete!"));
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

