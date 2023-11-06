// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerFoxState.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APlayerFoxState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool GetHasChosenCharacter() const;
	
	UFUNCTION(BlueprintCallable)
	void SetHasChosenCharacter(bool HasChosenChar);

private:
	UPROPERTY()
	bool bHasChosenCharacter = false;
};
