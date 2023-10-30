// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameState.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALobbyGameState : public ASideScrollerGameState
{
	GENERATED_BODY()

public:
	void PlayerSpawnCharacterSelect();

	UFUNCTION(BlueprintCallable)
	void OpenSelectCharacterMenu();
	
private:
	FTimerHandle CharacterSelectDelayTimerHandle;

	UPROPERTY(EditAnywhere)
	float CharacterSelectDelayTimer = 0.25;

protected:
	virtual void BeginPlay() override;
};
