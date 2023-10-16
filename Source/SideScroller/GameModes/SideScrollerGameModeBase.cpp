// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollerGameModeBase.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller/MenuSystem/MainMenu.h"
#include "UObject/ConstructorHelpers.h"

ASideScrollerGameModeBase::ASideScrollerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/Blueprints/Characters/Players/BP_PC_PlayerFox")
	);
	if (PlayerPawnBPClass.Class != nullptr)
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

void ASideScrollerGameModeBase::QuitGameHard() const
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

	if (const AGameModeBase* CurrentGameMode = Cast<AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		CurrentGameMode != nullptr && (
			CurrentGameMode->GetName().Contains("MainMenu") ||
			CurrentGameMode->GetName().Contains("GameOver")
		)
	) return;  // early return if in mainmenu or gameover menu
	
	if (Players.Num() < 1)
	{
		const UWorld* World = GetWorld();
		if (!World) return;
		
		// go to game over screen/level where you can either start over or exit.
		if (USideScrollerGameInstance* SideScrollerGameInstance =
			Cast<USideScrollerGameInstance>(World->GetGameInstance())
		) {
			SideScrollerGameInstance->LoadGameOverMenu();
		} else {
			UE_LOG(LogGameMode, Display,
				TEXT("ASideScrollerGameModeBase::Tick - Cant find sidescroller game instance; quitting game!")
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

TArray<APC_PlayerFox*> ASideScrollerGameModeBase::GetPlayers() const
{
	return Players;
}

void ASideScrollerGameModeBase::EnablePlayerGameModeInput(APlayerController* NewPlayer)
{
	if (NewPlayer != nullptr)
	{
		UE_LOG(LogGameMode, Display, TEXT("Setting input mode for Player, %s!"), *NewPlayer->GetName());
		NewPlayer->SetShowMouseCursor(false);
		NewPlayer->SetInputMode(FInputModeGameOnly());
	}
}

void ASideScrollerGameModeBase::EnableAllPlayersGameModeInput() const
{
	for (FConstPlayerControllerIterator PlayerControllersIterator = GetWorld()->GetPlayerControllerIterator();
		PlayerControllersIterator; ++PlayerControllersIterator
	) {
		if (TWeakObjectPtr<APlayerController> WeakPlayerController = *PlayerControllersIterator;
			WeakPlayerController.Get()
		) {
			APlayerController* PlayerController = WeakPlayerController.Get();
			UE_LOG(LogGameMode, Display, TEXT("Setting input mode for Player, %s!"), *PlayerController->GetName());
			PlayerController->SetShowMouseCursor(false);
			PlayerController->SetInputMode(FInputModeGameOnly());
		}	
	}
}