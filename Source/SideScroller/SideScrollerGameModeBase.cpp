// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollerGameModeBase.h"

#include "VectorTypes.h"
#include "Characters/Players/PC_PlayerFox.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ASideScrollerGameModeBase::ASideScrollerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_PC_PlayerFox"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASideScrollerGameModeBase::BeginPlay()
{
	/**
	Play the background music at the beginning of the game.
	*********************************************************************************/
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(AActor::GetWorld(), BackgroundMusic);
}

void ASideScrollerGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Players.Num() < 1)
	{
		UE_LOG(LogGameMode, Warning, TEXT("All players out of lives, quiting game!"));

		// TODO: go to game over screen/level where you can either start over or exit.

		// for now quiting the game
		const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
		UKismetSystemLibrary::QuitGame(
			GetWorld(),
			UGameplayStatics::GetPlayerController(GetWorld(), 0),
			QuitPreference,
			true
		);
	}
}

void ASideScrollerGameModeBase::AddPlayer(APC_PlayerFox* PlayerFox)
{
	this->Players.Add(PlayerFox);
}

void ASideScrollerGameModeBase::RemovePlayer(APC_PlayerFox* PlayerFox)
{
	if (Players.Contains(PlayerFox))
	{
		this->Players.Remove(PlayerFox);
	} else {
		UE_LOG(LogGameMode, Warning,
			TEXT("Player, %s, not in Players array. Not removing from Players array!"), *PlayerFox->GetName()
		);
	}
}
