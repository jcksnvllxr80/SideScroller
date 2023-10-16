// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"

#include "Kismet/GameplayStatics.h"

void ALevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	EnableAllPlayersGameModeInput();
}
