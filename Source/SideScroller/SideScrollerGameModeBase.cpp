// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollerGameModeBase.h"

#include "VectorTypes.h"
#include "Characters/Players/PC_PlayerFox.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystem/MainMenu.h"
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

void ASideScrollerGameModeBase::QuitGameHard()
{
	// hard coded quit game 
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		QuitPreference,
		true
	);
}

void ASideScrollerGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (  // early return if in main menu
		const AGameModeBase* CurrentGameMode = Cast<AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		CurrentGameMode != nullptr && CurrentGameMode->GetName().Contains("MainMenu")
	) return;
	
	if (Players.Num() < 1)
	{
		// go to game over screen/level where you can either start over or exit.
		USideScrollerGameInstance* SideScrollerGameInstance = Cast<USideScrollerGameInstance>(
			GetWorld()->GetGameInstance()
		);
		if (SideScrollerGameInstance)
		{
			UE_LOG(LogGameMode, Display, TEXT("All players out of lives, switching to game over menu!"));
			SideScrollerGameInstance->LoadGameOverMenu();
		} else {
			UE_LOG(LogGameMode, Display,
				TEXT("ASideScrollerGameModeBase::Tick - Cant find sidescroller game instance; quiting game!")
			);
			QuitGameHard();
		}
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
