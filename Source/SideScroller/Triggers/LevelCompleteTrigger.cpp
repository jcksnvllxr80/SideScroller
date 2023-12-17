// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCompleteTrigger.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Controllers/GameModePlayerController.h"

/**
 * Begins playing the level complete trigger.
 */
void ALevelCompleteTrigger::BeginPlay()
{
	Super::BeginPlay();
	this->GetPlacementExtent().Set(DoorSize.X, DoorSize.Y, DoorSize.Z);
}

/**
 * Notifies when an actor begins to overlap with the level complete trigger.
 *
 * @param OtherActor The actor that is overlapping with the level complete trigger.
 */
void ALevelCompleteTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ALevelCompleteTrigger::NotifyActorBeginOverlap - Overlap LevelCompleteTrigger is not PC_PlayerFox.")
		);
		return;
	}
	
	UE_LOG(LogTemp, Display,
		TEXT("ALevelCompleteTrigger::NotifyActorBeginOverlap - PC_PlayerFox, %s, overlapping LevelCompleteTrigger."),
		*Player->GetName()
	);
	
	PrepForNextLevel(Player);
}

/**
 * \brief Prepares for the next level by performing the following steps:
 *  - Checks if the given player's controller is an instance of AGameModePlayerController. If not, logs a warning
 *  message and returns.
 *  - Invokes the DoLevelCompleteServerRPC function on the player to show the level complete banner or celebration.
 *  - Binds the CallNextLevelStart function to the StartNextLevelDelayDelegate, with the given GameModePlayerController
 *  as a parameter.
 *  - Sets a timer to call the StartNextLevelDelayDelegate function after a specified delay.
 *
 * \param Player The player whose controller will be checked and used in the preparation for the next level.
 */
void ALevelCompleteTrigger::PrepForNextLevel(APC_PlayerFox* Player)
{
	AGameModePlayerController* GameModePlayerController = Cast<AGameModePlayerController>(Player->GetController());
	if (GameModePlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT("ALevelCompleteTrigger::NextLevel - PlayerController is not a \"GameMode\" PC.")
		)
		return;
	}

	// show level complete banner / celebration
	Player->DoLevelCompleteServerRPC();

	StartNextLevelDelayDelegate.BindUFunction(
		this,
		FName("CallNextLevelStart"),
		GameModePlayerController
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		this->StartNextLevelDelayTimerHandle,
		StartNextLevelDelayDelegate,
		this->StartNextLevelDelayTimer,
		false
	);
}

/**
 * Calls the StartNextLevel() method on the given AGameModePlayerController object.
 *
 * @param GameModePlayerController The AGameModePlayerController object to call StartNextLevel() on.
 */
void ALevelCompleteTrigger::CallNextLevelStart(AGameModePlayerController* GameModePlayerController)
{
	GameModePlayerController->StartNextLevel();
}
