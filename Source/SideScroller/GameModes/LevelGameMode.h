// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameModeBase.h"
#include "LevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALevelGameMode : public ASideScrollerGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> DefaultCharacterBP = nullptr;

private:
	bool LocateChosenCharacter(FConstPlayerControllerIterator Iter);
	void SpawnPlayerChosenCharacters();

	UPROPERTY(EditAnywhere)
	float SpawnPlayerChosenCharDelayTimer = 0.05;
	
	FTimerHandle SpawnPlayerChosenCharDelayTimerHandle;

protected:
	virtual void BeginPlay() override;
};
