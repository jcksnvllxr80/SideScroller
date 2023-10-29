// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameModePlayerController.generated.h"

class APC_PlayerFox;
/**
 * 
 */
UCLASS()
class SIDESCROLLER_API AGameModePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SpawnPlayer(
		TSubclassOf<class APC_PlayerFox> PlayerBP,
		const FString& PlayerColorStr,
		APlayerController* PlayerController
	);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void CheckGameStartReqs();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void StartLevel();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void TravelToLevel();

private:
	UPROPERTY(EditAnywhere)
	FVector PlayerSpawnDropInHeight {0.f, 0.f, 50.f};

protected:
	virtual void BeginPlay() override;
};
