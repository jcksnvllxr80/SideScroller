// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameState.h"
#include "LevelGameState.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALevelGameState : public ASideScrollerGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OpenRespawnMenu();
};
