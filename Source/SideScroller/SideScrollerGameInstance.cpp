// Fill out your copyright notice in the Description page of Project Settings.


#include "SideScrollerGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"

USideScrollerGameInstance::USideScrollerGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("USideScrollerGameInstance::USideScrollerGameInstance - Constructor Called."))
	const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!MainMenuBPClass.Class) return;
	MainMenuClass = MainMenuBPClass.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!InGameMenuBPClass.Class) return;
	InGameMenuClass = InGameMenuBPClass.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> SettingsMenuBPClass(TEXT("/Game/MenuSystem/WBP_SettingsMenu"));
	if (!SettingsMenuBPClass.Class) return;
	SettingsMenuClass = SettingsMenuBPClass.Class;
}

void USideScrollerGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("USideScrollerGameInstance::Init - Init() Called."))

}

// void USideScrollerGameInstance::LoadMainMenu()
// {
// 	APlayerController* PlayerController = GetFirstLocalPlayerController();
// 	if (!PlayerController) return;
// 	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
// }

void USideScrollerGameInstance::LoadMenu()
{
	if (MainMenuClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Main Menu blueprint class %s."), *MainMenuClass->GetName());
		UMainMenu* Menu = CreateWidget<UMainMenu>(this, MainMenuClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMainMenu Menu from main menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the Main Menu blueprint class."));
		return;
	}
}

void USideScrollerGameInstance::PlayGame()
{
	UE_LOG(LogTemp, Warning, TEXT("USideScrollerGameInstance::PlayGame - leaving main menustarting game..."));
	UWorld* World = GetWorld();
	if (!World) return;
	// bUseSeamlessTravel = true;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}
