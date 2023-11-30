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
	UFUNCTION(BlueprintCallable)
	void LogPlayerCount(FString FunctionName) const;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	bool IsNumPlayersRequirementFulfilled() const;

	UFUNCTION(BlueprintCallable)
	void SetNumPlayersToStart();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

protected:
	virtual void BeginPlay() override;

private:
	int NumberOfPlayers = 0;
	int MinPlayersToStartGame = 6;
	bool bNumPlayersRequirementFulfilled = false;
};
