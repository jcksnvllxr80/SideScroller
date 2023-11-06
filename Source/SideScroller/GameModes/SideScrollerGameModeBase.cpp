// Copyright Epic Games, Inc. All Rights Reserved.


#include "SideScrollerGameModeBase.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
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
	) return;  // early return if in mainmenu or gameover menu, we dont want to quit in these cases

	PrintPlayersList();
	
	if (Players.Num() < 1)
	{
		UE_LOG(LogGameMode, Warning,
			TEXT("ASideScrollerGameModeBase::Tick - Number of Players less that 1; going to gameover menu!")
		);
		
		// go to game over screen/level where you can either start over or exit.
		if (USideScrollerGameInstance* SideScrollerGameInstance =
			Cast<USideScrollerGameInstance>(GetGameInstance())
		) {
			SideScrollerGameInstance->LoadGameOverMenu();
		} else {
			UE_LOG(LogGameMode, Warning,
				TEXT("ASideScrollerGameModeBase::Tick - Cant find sidescroller game instance; quitting game!")
			);
			QuitGameHard();
		}
	}
}

void ASideScrollerGameModeBase::PrintPlayersList()
{
	if (Players.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("ASideScrollerGameModeBase::PrintPlayersList - List is empty. Returning early."));
		return;
	}
	
	FString PlayerArrayStr = "";
	for (const APC_PlayerFox* Player : Players)
	{
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("ASideScrollerGameModeBase::PrintPlayersList - Found null Player."));
			continue;
		}
		PlayerArrayStr += (Player->GetName() + (Player->IsDead() ? ": Dead; " : ": Alive; "));
	}
	UE_LOG(LogTemp, Display, TEXT("List of Players is %s"), *PlayerArrayStr);
}

void ASideScrollerGameModeBase::Logout(AController* Exiting)
{
	APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(Exiting->GetPawn());
	if (PlayerFox != nullptr)
	{
		PlayerFox->DestroyActor();
		RemovePlayer(PlayerFox);
	}
	else
	{
		UE_LOG(LogGameMode, Warning,
			TEXT("ASideScrollerGameModeBase::Logout - PlayerFox is null. Not removing from Players array!"),
		);
	}
	
	Super::Logout(Exiting);
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
			TEXT("Player, %s, not in Players array. Not removing from Players array!"),
			*PlayerFox->GetPlayerName().ToString()
		);
	}
}

TArray<APC_PlayerFox*> ASideScrollerGameModeBase::GetPlayers() const
{
	return Players;
}

void ASideScrollerGameModeBase::TravelToGameOverMenu()
{
	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/Maps/Map_GameOverMenu?listen");
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
