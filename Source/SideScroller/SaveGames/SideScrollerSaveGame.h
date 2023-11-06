// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SideScrollerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API USideScrollerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	float VolumeLevel;

	UPROPERTY()
	int ResolutionIndex;
	
	USideScrollerSaveGame();
};