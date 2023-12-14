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
