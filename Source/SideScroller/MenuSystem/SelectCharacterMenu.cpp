// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterMenu.h"
#include "Components/Button.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
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

void USelectCharacterMenu::UpdateSelectedCharacterButtons()
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - No update. Cant find World.")
		);
		return;
	}
	
	AGameStateBase* GameState = World->GetGameState();
	if (GameState == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - No update. Cant find GameState.")
		);
		return;
	}

	TArray<FString> CurrentPlayerPawnNames;
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - Updating button statuses."));
	for (const APlayerState* PlayerStateI : GameState->PlayerArray)
	{
		const APawn* PlayerPawnI = PlayerStateI->GetPawn();
		if (PlayerPawnI == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - Cant get Pawn related to PlayerState %s."),
				*PlayerStateI->GetName()
			);
			continue;
		}
		CurrentPlayerPawnNames.Add(PlayerPawnI->GetName());
	}

	std::map<FString, UButton*>::iterator Iterator = CharacterColorButtonMap.begin();
	while (Iterator != CharacterColorButtonMap.end())
	{
		// Accessing the key
		FString ButtonColor = Iterator->first;
		// Accessing the value
		UButton* Button = Iterator->second;

		UE_LOG(LogTemp, VeryVerbose,
			TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - Enabling, the %s button associated with "
				"the %s charater. If a player is using this character, it will be disabled again."
			),
			*Button->GetName(),
			*ButtonColor
		);
		Button->SetIsEnabled(true);
		
		// look through the CurrentPlayerPawnNames to see if each button color is in the name
		for (FString PlayerPawnName : CurrentPlayerPawnNames)
		{
			if (PlayerPawnName.Contains(ButtonColor))
			{
				UE_LOG(LogTemp, Verbose,
					TEXT("USelectCharacterMenu::UpdateSelectedCharacterButtons - "
						"Disabling %s as %s is using the %s charater."
					),
					*Button->GetName(),
					*PlayerPawnName,
					*ButtonColor
				);
				Button->SetIsEnabled(false);
			}
		}
		
		// iterator incremented to point next item
		++Iterator;
	}
}

bool USelectCharacterMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;

	if (PinkPlayerButton)
	{
		PinkPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::PinkPlayerSelect);
		PinkPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		PinkPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find PinkPlayer Button."));
		return false;
	}

	if (OrangePlayerButton)
	{
		OrangePlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::OrangePlayerSelect);
		OrangePlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		OrangePlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find OrangePlayer Button."));
		return false;
	}

	if (YellowPlayerButton)
	{
		YellowPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::YellowPlayerSelect);
		YellowPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		YellowPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find YellowPlayer Button."));
		return false;
	}

	if (GreenPlayerButton)
	{
		GreenPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::GreenPlayerSelect);
		GreenPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		GreenPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find GreenPlayer Button."));
		return false;
	}

	if (BluePlayerButton)
	{
		BluePlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BluePlayerSelect);
		BluePlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		BluePlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find BluePlayer Button."));
		return false;
	}

	if (BlackPlayerButton)
	{
		BlackPlayerButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BlackPlayerSelect);
		BlackPlayerButton->OnHovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
		BlackPlayerButton->OnUnhovered.AddDynamic(this, &USelectCharacterMenu::UpdateSelectedCharacterButtons);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find BlackPlayer Button."));
		return false;
	}
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BackToGame);
		
		CancelButton->SetIsEnabled(false);  // disabled if the player hasn't selected a character yet
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("USelectCharacterMenu::Initialize - No PlayerController. Not enabling cancel button.")
			);
		}
		else
		{
			APC_PlayerFox* PlayerFox = dynamic_cast<APC_PlayerFox*>(PlayerController->GetPawn());
			if (PlayerFox == nullptr)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("USelectCharacterMenu::Initialize - No PlayerFox. cant check if player selcted character")
				);
			}
			else if (PlayerFox->GetHasChosenCharacter())
			{
				UE_LOG(LogTemp, Display,
				       TEXT("USelectCharacterMenu::Initialize - Player has selected character, enabling cancel button.")
				);
				CancelButton->SetIsEnabled(true);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find Cancel button."));
		return false;
	}
	
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &USelectCharacterMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USelectCharacterMenu::Initialize - INIT FAILED! Cant find Exit button."));
		return false;
	}

	CharacterColorButtonMap = {
		{"Pink",			PinkPlayerButton},
		{"PC_PlayerFox_C",	OrangePlayerButton},
		{"Yellow",			YellowPlayerButton},
		{"Green",			GreenPlayerButton},
		{"Blue",			BluePlayerButton},
		{"Black",			BlackPlayerButton}
	};

	UpdateSelectedCharacterButtons();
	
	UE_LOG(LogTemp, Display, TEXT("USelectCharacterMenu::Initialize - Select Character Menu Init complete!"));
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

bool USelectCharacterMenu::WriteLaunchWarningOnScreen(APlayerController* PlayerController)
{
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr)
	{
		UEngine* Engine = GameInstance->GetEngine();
		if (!Engine) return true;
		Engine->AddOnScreenDebugMessage(0,
            CheckStartReadinessDelayTimer,
            FColor::Green,
            TEXT("USelectCharacterMenu::SelectPlayer - Checking to see if all players are ready, if so "
                "the game will be launching in a few seconds"
            )
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("USelectCharacterMenu::SelectPlayer - Can't show onscreen message. GameInstance is null!")
		);
	}
	return false;
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

	CheckStartReadinessDelayTimerDelegate.BindUFunction(
		this,
		FName("CheckStartReadinessDelay"),
		GameModePlayerController
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		this->CheckStartReadinessDelayTimerHandle,
		CheckStartReadinessDelayTimerDelegate,
		CheckStartReadinessDelayTimer,
		false
	);

	if (WriteLaunchWarningOnScreen(PlayerController)) return;
}

void USelectCharacterMenu::CheckStartReadinessDelay(AGameModePlayerController* GameModePlayerController)
{
	GameModePlayerController->CheckGameStartReqs();
	GameModePlayerController->TravelToLevel();
}
