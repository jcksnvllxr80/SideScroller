// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALobbyGameMode : public ASideScrollerGameModeBase
{
	GENERATED_BODY()

public:
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

private:
	uint32 NumberOfPlayers = 0;

	UPROPERTY(EditAnywhere)
	uint32 MinPlayersToStartGame = 3;
};
