// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SideScrollerGameState.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ASideScrollerGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int GetCurrentLevel() const;
	
	UFUNCTION(BlueprintCallable)
	void OpenRespawnMenu();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	int CurrentLevel = 0;
};
