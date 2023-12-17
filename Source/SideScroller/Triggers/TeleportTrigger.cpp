// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportTrigger.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Interactables/Door.h"

/**
 * @brief Constructor for the ATeleportTrigger class.
 *
 * Sets the primary actor tick to be able to tick.
 */
ATeleportTrigger::ATeleportTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * This method is called when the game starts or when the object is spawned into the world. It is responsible
 * for initializing any required variables and performing any necessary setup tasks.
 */
void ATeleportTrigger::BeginPlay()
{
	Super::BeginPlay();
	GlobalTeleportTargetLocation = GetTransform().TransformPosition(TeleportTargetLocation);
}

/**
 * Executes the Tick functionality of the ATeleportTrigger class.
 *
 * This method is called every frame and is responsible for checking if the player
 * is overlapping the teleport trigger and if the corresponding teleport source door is open.
 * If both conditions are met, the player is prepared for teleportation and the
 * bPlayerIsOverlappingTrigger flag is set to false.
 *
 * @param DeltaTime The time in seconds it took to complete the last frame.
 */
void ATeleportTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlayerIsOverlappingTrigger && TeleportSourceDoor->GetIsOpen() && OverlappingPlayer != nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ATeleportTrigger::Tick - PC_PlayerFox, %s, overlapping TeleportTrigger and door is open. Teleporting"),
			*OverlappingPlayer->GetPlayerName().ToString()
		);
		PrepForTeleport(OverlappingPlayer);
		bPlayerIsOverlappingTrigger = false;
	}
}

/**
 * Notifies when an actor begins to overlap with the teleport trigger.
 *
 * @param OtherActor The actor that is overlapping with the teleport trigger.
 */
void ATeleportTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ATeleportTrigger::NotifyActorBeginOverlap - Overlap TeleportTrigger is not PC_PlayerFox.")
		);
		return;
	}

	bPlayerIsOverlappingTrigger = true;
	OverlappingPlayer = Player;
	
	if (TeleportSourceDoor->GetIsOpen())
	{
		UE_LOG(LogTemp, Display,
			TEXT("ATeleportTrigger::NotifyActorBeginOverlap - PC_PlayerFox, %s, overlapping TeleportTrigger."),
			*Player->GetPlayerName().ToString()
		);
		
		PrepForTeleport(Player);
	}
	else
	{
		UE_LOG(LogTemp, Display,
			TEXT("ATeleportTrigger::NotifyActorBeginOverlap - Door, %s, is not open, not teleporting player, %s."
				" Entering loop to check if door is open while player is still overlapping the TeleportTrigger"),
			*TeleportSourceDoor->GetName(),
			*Player->GetPlayerName().ToString()
		);
	}
}

/**
 
 \brief Notifies when an actor ends overlap with the TeleportTrigger.
 
 \param OtherActor The actor that ended overlap with the TeleportTrigger.
 
 \details This method is called when an actor ends its overlap with the TeleportTrigger. It first calls the
 parent implementation of the method using the Super keyword. It then checks if the OtherActor is of type
 APC_PlayerFox by performing a dynamic cast. If the dynamic cast fails and returns nullptr, the method simply
 returns without performing any further actions
 *.
 
 \details If the OtherActor is of type APC_PlayerFox, it logs a message using the UE_LOG macro and sets the
 bPlayerIsOverlappingTrigger flag to false and the OverlappingPlayer property to nullptr.
 
 \note This method should be overridden in child classes to add any additional functionality.
 **/
void ATeleportTrigger::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	const APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
	if (Player == nullptr) return;
	
	UE_LOG(LogTemp, Display,
		TEXT("ATeleportTrigger::NotifyActorEndOverlap - PC_PlayerFox, %s, no longer overlapping TeleportTrigger."),
		*Player->GetPlayerName().ToString()
	);

	bPlayerIsOverlappingTrigger = false;
	OverlappingPlayer = nullptr;	
}

/**
 * Plays the teleport sound for the given player.
 *
 * @param Player: The player to play the sound for.
 */
void ATeleportTrigger::PlayTeleportSound(const APC_PlayerFox* Player)
{
	UGameplayStatics::SpawnSoundAttached(
		this->TeleportSound,
		Player->GetSprite(),
		TEXT("BasePaperCharacterPain")
	);
}

/**
 * Prepares for teleporting the player.
 *
 * This method prepares for teleporting the player by binding the 'Teleport' function as a delegate
 * and setting a timer to delay the teleportation.
 *
 * @param Player Pointer to the APC_PlayerFox instance representing the player to be teleported.
 */
void ATeleportTrigger::PrepForTeleport(const APC_PlayerFox* Player)
{
	TeleportDelayDelegate.BindUFunction(
		this,
		FName("Teleport"),
		Player
	);
	
	GetWorld()->GetTimerManager().SetTimer(
		this->TeleportDelayTimerHandle,
		TeleportDelayDelegate,
		this->TeleportDelayTimer,
		false
	);
}

/**
 * Teleports the specified player to the global teleport target location.
 *
 * @param Player The player to teleport.
 */
void ATeleportTrigger::Teleport(APC_PlayerFox* Player)
{
	UE_LOG(LogTemp, Display,
		TEXT("ATeleportTrigger::Teleport - Teleporting player, %s, to %s."),
		*Player->GetPlayerName().ToString(),
		*GlobalTeleportTargetLocation.ToString()
	);
	
	PlayTeleportSound(Player);
	
	Player->SetActorLocation(
		GlobalTeleportTargetLocation, false, nullptr, ETeleportType::ResetPhysics
	);
}
