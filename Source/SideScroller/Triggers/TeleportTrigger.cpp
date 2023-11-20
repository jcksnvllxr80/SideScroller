// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportTrigger.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Interactables/Door.h"

ATeleportTrigger::ATeleportTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATeleportTrigger::BeginPlay()
{
	Super::BeginPlay();
	GlobalTeleportTargetLocation = GetTransform().TransformPosition(TeleportTargetLocation);
}

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

void ATeleportTrigger::PlayTeleportSound(const APC_PlayerFox* Player)
{
	UGameplayStatics::SpawnSoundAttached(
		this->TeleportSound,
		Player->GetSprite(),
		TEXT("BasePaperCharacterPain")
	);
}

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
