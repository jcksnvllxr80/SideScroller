// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelGameState.h"
#include "Level2GameState.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALevel2GameState : public ALevelGameState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
