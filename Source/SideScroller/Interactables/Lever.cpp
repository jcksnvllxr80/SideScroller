// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Mechanics/PlatformBlocks/MovingPlatform.h"

ALever::ALever()
{
	InteractableBox->SetRelativeScale3D(FVector(0.5,0.4,0.2));
	InteractableBox->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

void ALever::Interact()
{
	if (GetCanInteract())
	{
		SetCanInteract(false);
		ToggleLever();
	}
}

void ALever::TurnOffLever()
{
	UE_LOG(LogTemp, Display, TEXT("ALever::ToggleLever - Setting lever to off"))
	bIsTrue = false;
	InteractableFlipbook->SetFlipbook(FalsePosition);

	for (AMovingPlatform* Platform: PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}

void ALever::TurnOnLever()
{
	UE_LOG(LogTemp, Display, TEXT("ALever::ToggleLever - Setting lever to on"))
	bIsTrue = true;
	InteractableFlipbook->SetFlipbook(TruePosition);
	
	for (AMovingPlatform* Platform: PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void ALever::MoveLever()
{
	if (bIsTrue)
	{
		TurnOffLever();
	}
	else
	{
		TurnOnLever();
	}

	SetCanInteract(true);
}

void ALever::PlayLeverMoveSound() const
{
	UGameplayStatics::SpawnSoundAttached(
		this->LeverMoveSound,
		this->InteractableFlipbook,
		TEXT("LeverMoveSound")
	);
}

void ALever::ToggleLever_Implementation()
{
	PlayLeverMoveSound();

	GetWorld()->GetTimerManager().SetTimer(
		this->LeverMoveTimerHandle,
		this,
		&ALever::MoveLever,
		LeverMoveTime,
		false
	);
}

bool ALever::ToggleLever_Validate()
{
	return true;
}
