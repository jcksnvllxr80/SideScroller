// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScrollerGameState.h"

#include "SideScroller/SideScrollerGameInstance.h"

void ASideScrollerGameState::BeginPlay()
{
	Super::BeginPlay();
	
}

int ASideScrollerGameState::GetCurrentLevel() const
{
	return CurrentLevel;
}

void ASideScrollerGameState::OpenRespawnMenu()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance != nullptr) {
		GameInstance->RespawnLoadMenu();
	} else {
		UE_LOG(LogTemp, Warning,
			TEXT("ASideScrollerGameState::OpenRespawnMenu - Cant find GameInstance!")
		);
	}
}
