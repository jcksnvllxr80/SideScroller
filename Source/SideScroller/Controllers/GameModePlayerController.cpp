// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlayerController.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/GameModes/LobbyGameMode.h"

void AGameModePlayerController::TravelToLevel_Implementation()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::TravelToLevel_Implementation - No GameInstance."),
		);
		return;  // no game instance - early return
	}
	
	if (GameInstance->IsReadyToStartGame())
	{
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::TravelToLevel_Implementation - Starting game! All players ready.")
		);
		this->StartLevel();
	}
	else
	{
		UE_LOG(LogTemp, Display,
			TEXT("USelectCharacterMenu::SelectPlayer - Not starting game! All players not ready.")
		);
	}
}

bool AGameModePlayerController::TravelToLevel_Validate()
{
	return true;  // This will allow the RPC to be called
}

void AGameModePlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->SetShowMouseCursor(false);
	this->SetInputMode(FInputModeGameOnly());
}

void AGameModePlayerController::SpawnPlayer_Implementation(
	TSubclassOf<APC_PlayerFox> PlayerBP,
	const FString& PlayerColorStr,
	APlayerController* PlayerController
) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;
	if (PlayerBP != nullptr)
	{
		if (USideScrollerGameInstance* SideScrollerGameInstance =
			Cast<USideScrollerGameInstance>(GetWorld()->GetGameInstance());
			SideScrollerGameInstance != nullptr
		) {
			SideScrollerGameInstance->SetChosenCharacter(PlayerController, PlayerBP);
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("ASideScrollerGameModeBase::SpawnPlayer - Cant find GameInstance"))
		}
		
		APC_PlayerFox* NewCharacter = GetWorld()->SpawnActor<APC_PlayerFox>(
			PlayerBP->GetDefaultObject()->GetClass(),
			PlayerController->GetPawn()->GetActorLocation() + PlayerSpawnDropInHeight,
			PlayerController->GetPawn()->GetActorRotation(),
			SpawnParams
		);
		
		if (NewCharacter)
		{
			APawn* PawnToBeReplaced = PlayerController->GetPawn();
			PlayerController->UnPossess();
			PlayerController->Possess(NewCharacter);
			if (PawnToBeReplaced) PawnToBeReplaced->Destroy();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error,
			   TEXT("ASideScrollerGameModeBase::SpawnPlayer - Select %s Character Failed! No %sPlayerBP."),
			   *PlayerColorStr, *PlayerColorStr
		);
	}
}


bool AGameModePlayerController::SpawnPlayer_Validate(
	TSubclassOf<APC_PlayerFox> PlayerBP,
	const FString& PlayerColorStr,
	APlayerController* PlayerController
) {
	return true;  // This will allow the RPC to be called
}

void AGameModePlayerController::CheckGameStartReqs_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Game mode is not LobbyGameMode.")
		)
		return;  // need more players to start - early return - dont start game
	}
	
	if (!LobbyGameMode->IsNumPlayersRequirementFulfilled())
	{
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Not enough players to start game.")
		)
		return;
	}
	
	USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Cant find GameInstance.")
		)
		return;
	}
	
	if (!GameInstance->IsEveryPlayersCharacterChosen())
	{
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation "
				"- Not starting game, not all players have chosen theiir characters."
			)
		)
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Start game, players ready.")
	)
	GameInstance->SetReadyToStartGame(true);
}

bool AGameModePlayerController::CheckGameStartReqs_Validate()
{
	return true;  // This will allow the RPC to be called
}

void AGameModePlayerController::StartLevel_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("AGameModePlayerController::StartLevel_Implementation - Game mode is not LobbyGameMode.")
		)
		return;  // need more players to start - early return - dont start game
	}

	LobbyGameMode->StartGame();
}

bool AGameModePlayerController::StartLevel_Validate()
{
	return true;  // This will allow the RPC to be called
}
