// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePlayerController.h"

void AGameModePlayerController::BeginPlay()
{
	Super::BeginPlay();
	this->SetShowMouseCursor(false);
	this->SetInputMode(FInputModeGameOnly());
}
