// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
	InteractableBox->SetRelativeScale3D(FVector(0.5,0.4,0.5));
	InteractableBox->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

void ADoor::Interact()
{
	if (GetCanInteract())
	{
		SetCanInteract(false);
		ToggleDoor();
	}
}

void ADoor::ToggleDoor()
{
	if (bIsTrue)
	{
		CloseDoorSoundAndTimer();
	}
	else
	{
		OpenDoorSoundAndTimer();
	}
}

void ADoor::CloseDoor()
{
	UE_LOG(LogTemp, Display, TEXT("ADoor::CloseDoor - Setting door to closed"))
	bIsTrue = false;
	InteractableFlipbook->SetFlipbook(FalsePosition);

	SetCanInteract(true);
}

void ADoor::OpenDoor()
{
	UE_LOG(LogTemp, Display, TEXT("ADoor::OpenDoor - Setting door to open"))
	bIsTrue = true;
	InteractableFlipbook->SetFlipbook(TruePosition);

	SetCanInteract(true);
}

void ADoor::PlayDoorSound_Implementation(USoundBase* DoorSound) const
{
	UGameplayStatics::SpawnSoundAttached(
		DoorSound,
		this->InteractableFlipbook,
		TEXT("DoorSound")
	);
}

void ADoor::CloseDoorSoundAndTimer()
{
	PlayDoorSound(DoorCloseSound);

	GetWorld()->GetTimerManager().SetTimer(
		this->DoorCloseTimerHandle,
		this,
		&ADoor::CloseDoor,
		DoorCloseTime,
		false
	);
}

void ADoor::OpenDoorSoundAndTimer()
{
	PlayDoorSound(DoorOpenSound);

	GetWorld()->GetTimerManager().SetTimer(
		this->DoorOpenTimerHandle,
		this,
		&ADoor::OpenDoor,
		DoorOpenTime,
		false
	);
}

bool ADoor::GetIsOpen() const
{
	return bIsTrue;
}
