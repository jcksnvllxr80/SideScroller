// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Players/PC_PlayerFox.h"
#include "GameFramework/GameModeBase.h"
#include "SideScrollerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ASideScrollerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASideScrollerGameModeBase();
	
	virtual void BeginPlay() override;
	
	void QuitGameHard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Game)
	void AddPlayer(APC_PlayerFox* PlayerFox);

	UFUNCTION(BlueprintCallable, Category = Game)
	void RemovePlayer(APC_PlayerFox* PlayerFox);

	// The sound cue to play for the background music.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	USoundBase* BackgroundMusic = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = Game)
	TArray<APC_PlayerFox*> Players;
};
