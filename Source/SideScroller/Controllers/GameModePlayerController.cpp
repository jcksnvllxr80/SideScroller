// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlayerController.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

void AGameModePlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->SetShowMouseCursor(false);
	this->SetInputMode(FInputModeGameOnly());
}

void AGameModePlayerController::SpawnPlayer_Implementation(TSubclassOf<APC_PlayerFox> PlayerBP,
	const FString& PlayerColorStr, APlayerController* PlayerController)
{
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


bool SpawnPlayer_Validate()
{
	return true;                              // This will allow the RPC to be called
}