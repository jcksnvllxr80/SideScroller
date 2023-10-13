// Fill out your copyright notice in the Description page of Project Settings.


#include "FallOffLevelTrigger.h"

#include "Characters/Players/PC_PlayerFox.h"

void AFallOffLevelTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Overlap of FallOffLevelTrigger is not PC_PlayerFox."));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("PC_PlayerFox, %s, overlapping falloff trigger."), *Player->GetName());
		Player->HandleFallOffLevel();
	}
}
