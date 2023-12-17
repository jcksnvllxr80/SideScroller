// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointTrigger.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/GameModes/SideScrollerGameModeBase.h"

/**
 * Constructor for the ACheckpointTrigger class.
 */
ACheckpointTrigger::ACheckpointTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	CheckpointFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CheckpointPaperFlipbook"));
	CheckpointFlipbook->SetupAttachment(RootComponent);

	this->CheckpointBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckpointBox"));
	this->CheckpointBox->SetupAttachment(CheckpointFlipbook);
	this->CheckpointBox->SetHiddenInGame(true);
	this->CheckpointBox->SetRelativeScale3D(FVector(0.4,0.4,0.4));
	this->CheckpointBox->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

/**
 * ACheckpointTrigger::Tick
 *
 * This method is responsible for updating the state of the checkpoint trigger every frame.
 * It calls the parent's Tick method and applies spin animation to the checkpoint flipbook if enabled.
 *
 * @param DeltaTime - The time since the last frame.
 */
void ACheckpointTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CheckpointFlipbook && this->bSpin)
	{
		CheckpointFlipbook->AddLocalRotation(
			FRotator(0.f, CheckPointSpinForce * DeltaTime, 0.f),
			false,
			nullptr,
			ETeleportType::None
		);
	}
}

/**
 * @brief Plays the beginning of the level.
 *
 * This method is called when the level begins. It initializes the checkpoint trigger
 * and sets up the necessary components and events for the trigger to function properly.
 */
void ACheckpointTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (!CheckpointFlipbook) return;
	
	this->CheckpointFlipbook->SetFlipbook(IdleCheckpoint);
	
	this->CheckpointBox->SetGenerateOverlapEvents(true);
	this->CheckpointBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointTrigger::OnBeginOverlapDelegate);
}

/**
 * Starts the spinning animation of the flipbook.
 *
 * @param SpinTime The duration in seconds for which the flipbook should spin.
 */
void ACheckpointTrigger::SpinFlipbook()
{
	this->bSpin = true;
	GetWorld()->GetTimerManager().SetTimer(
		this->SpinTimerHandle,
		this,
		&ACheckpointTrigger::DestroyActor,
		SpinTime,
		false
	);
}

/**
 * @brief Sets the checkpoint location for all players.
 *
 * This method retrieves the game mode and gets all the current players.
 * It then iterates over each player and sets their last checkpoint location
 * to the location of the checkpoint flipbook component.
 *
 * @note If a player is found to be null in the players array, a warning will be logged
 *       and the player's last checkpoint location won't be updated.
 *
 * @param None
 *
 * @return None
 */
void ACheckpointTrigger::SetAllPlayersCheckpointLocations() const
{
	const ASideScrollerGameModeBase* GameMode = UECasts_Private::DynamicCast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode()
	);
	if (GameMode != nullptr)
	{
		TArray<APC_PlayerFox*> CurrentPlayers = GameMode->GetPlayers();
		for (APC_PlayerFox* CurrentPlayer : CurrentPlayers)
		{
			if (CurrentPlayer == nullptr)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("ACheckpointTrigger::SetAllPlayersCheckpointLocationsOverlap - A Player exists in the "
						"PlayersArray but is null. Not updating this player's LastCheckpointLocation."
					)
				);
				return;	
			}
			CurrentPlayer->SetLastCheckpointLocation(this->CheckpointFlipbook->GetComponentLocation());
		}
	}
}

/**
 * @brief This method is called when an actor begins to overlap with the CheckpointTrigger.
 *
 * @param OverlappedComponent The primitive component that triggered the overlap.
 * @param OtherActor The actor that is overlapping with the CheckpointTrigger.
 * @param OtherComp The primitive component of the other actor involved in the overlap.
 * @param OtherBodyIndex The body index of the other actor involved in the overlap.
 * @param bFromSweep Indicates if the overlap was triggered by a sweep.
 * @param SweepResult The hit result of the sweep overlap.
 */
void ACheckpointTrigger::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	const APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Overlap of CheckpointTrigger is not PC_PlayerFox."));
		return;
	}
	
	if (!bHasGivenFeedback)
	{
		this->bHasGivenFeedback = true;
		CheckpointFeedback(OverlappedComponent);
		UE_LOG(LogTemp, Display, TEXT("PC_PlayerFox, %s, overlapping CheckpointTrigger."), *Player->GetName());

		SetAllPlayersCheckpointLocations();
	}
}

/**
 * This method is used to provide visual and audio feedback when a checkpoint is triggered.
 *
 * @param OverlappedComponent The primitive component that triggered the checkpoint.
 */
void ACheckpointTrigger::CheckpointFeedback(UPrimitiveComponent* OverlappedComponent)
{
	SpinFlipbook();
	
	UGameplayStatics::SpawnSoundAttached(
		this->CheckpointSound,
		OverlappedComponent,
		TEXT("BasePickupSound")
	);
}

/**
 * @brief Destroy the actor and clear the spin timer.
 *
 */
void ACheckpointTrigger::DestroyActor()
{
	UE_LOG(LogTemp, Display, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->SpinTimerHandle);
}
