// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

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
}

void ALever::TurnOnLever()
{
	UE_LOG(LogTemp, Display, TEXT("ALever::ToggleLever - Setting lever to on"))
	bIsTrue = true;
	InteractableFlipbook->SetFlipbook(TruePosition);
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

void ALever::ToggleLever()
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
