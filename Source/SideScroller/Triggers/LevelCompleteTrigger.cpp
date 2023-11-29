// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCompleteTrigger.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Controllers/GameModePlayerController.h"

void ALevelCompleteTrigger::BeginPlay()
{
	Super::BeginPlay();
	this->GetPlacementExtent().Set(DoorSize.X, DoorSize.Y, DoorSize.Z);
}

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
	Player->DoLevelComplete();

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

void ALevelCompleteTrigger::CallNextLevelStart(AGameModePlayerController* GameModePlayerController)
{
	GameModePlayerController->StartNextLevel();
}
