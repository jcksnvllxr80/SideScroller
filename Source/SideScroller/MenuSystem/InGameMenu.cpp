// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Cancel button during init."));
		return false;
	}
	
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Exit button during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("In Game Menu Init complete!"));
	return true;
}

void UInGameMenu::BackToMainMenu()
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

void UInGameMenu::BackToGame()
{
	OnLevelRemovedFromWorld();
}

