// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlayerController.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/GameModes/LobbyGameMode.h"
#include "SideScroller/PlayerStates/PlayerFoxState.h"

void AGameModePlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->SetShowMouseCursor(false);
	this->SetInputMode(FInputModeGameOnly());
}

void AGameModePlayerController::TravelToLevel_Implementation()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::TravelToLevel_Implementation - No GameInstance.")
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

void AGameModePlayerController::SpawnPlayer_Implementation(
	TSubclassOf<APC_PlayerFox> PlayerBP,
	const FString& PlayerColorStr,
	APlayerController* PlayerController
) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;
	if (PlayerBP == nullptr)
	{
		UE_LOG(LogTemp, Error,
			   TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Not spawning %s char. No %sPlayerBP."),
			   *PlayerColorStr
		);
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Not spawning %s character. Cant find World."),
			*PlayerController->GetName()
		)
		return;  // dont go any further, cant find world
	}

	USideScrollerGameInstance* SideScrollerGameInstance = Cast<USideScrollerGameInstance>(World->GetGameInstance());
	if (SideScrollerGameInstance != nullptr)
	{
		SideScrollerGameInstance->SetChosenCharacter(PlayerController, PlayerBP);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Cant save chosen char. No GameInstance")
		);
	}
	
	APawn* PlayerControllerPawn = PlayerController->GetPawn();
	if (PlayerControllerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - Cant spawn new pawn. %s's pawn not found."),
			*PlayerController->GetName()
		);
		return;
	}
	
	APC_PlayerFox* NewCharacter = World->SpawnActor<APC_PlayerFox>(
		PlayerBP->GetDefaultObject()->GetClass(),
		PlayerControllerPawn->GetActorLocation() + PlayerSpawnDropInHeight,
		PlayerControllerPawn->GetActorRotation(),
		SpawnParams
	);
	
	if (NewCharacter)
	{
		APawn* PawnToBeReplaced = PlayerControllerPawn;

		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - PlayerController, %s, unpossessing old pawn"),
			*PlayerController->GetName()
		);
		PlayerController->UnPossess();

		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - PlayerController, %s, possessing new pawn"),
			*PlayerController->GetName()
		);
		PlayerController->Possess(NewCharacter);
		
		APlayerFoxState* PlayerFoxState = Cast<APlayerFoxState>(NewCharacter->GetPlayerState());
		if (PlayerFoxState != nullptr)
		{
			PlayerFoxState->SetHasChosenCharacter(true);	
		}
		
		UE_LOG(LogTemp, Display,
			TEXT("AGameModePlayerController::SpawnPlayer_Implementation - PlayerController, %s, destroying old Pawn"),
			*PlayerController->GetName()
		);
		if (PawnToBeReplaced)
		{
			APC_PlayerFox* PlayerPawnToBeDestroyed = Cast<APC_PlayerFox>(PawnToBeReplaced);
			if (PlayerPawnToBeDestroyed != nullptr)
			{
				PlayerPawnToBeDestroyed->DestroyActor();
			}

			PawnToBeReplaced->Destroy();
		}
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
		TEXT("AGameModePlayerController::CheckGameStartReqs_Implementation - Player Prereqs met, game can start.")
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
