// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCompleteTrigger.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

void ALevelCompleteTrigger::BeginPlay()
{
	Super::BeginPlay();
	this->GetPlacementExtent().Set(DoorSize.X, DoorSize.Y, DoorSize.Z);
}

void ALevelCompleteTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Overlap of LevelCompleteTrigger is not PC_PlayerFox."));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("PC_PlayerFox, %s, overlapping LevelCompleteTrigger."), *Player->GetName());
	
	// TODO: use RPC to go to next level in case client got to the door first
}
