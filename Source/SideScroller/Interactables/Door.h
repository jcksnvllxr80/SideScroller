// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ADoor : public ABaseInteractable, public IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Interact() override;

	UFUNCTION()
	void CloseDoorSoundAndTimer();

	UFUNCTION()
	void OpenDoorSoundAndTimer();

private:
	UFUNCTION()
	void ToggleDoor();

	UFUNCTION()
	void CloseDoor();
	
	UFUNCTION()
	void OpenDoor();

	UFUNCTION()
	void PlayDoorSound(USoundBase* DoorSound) const;
	
	UPROPERTY(EditAnywhere)
	USoundBase* DoorOpenSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* DoorCloseSound;
	
	FTimerHandle DoorOpenTimerHandle;

	FTimerHandle DoorCloseTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float DoorOpenTime = 0.75;

	UPROPERTY(EditAnywhere)
	float DoorCloseTime = 0.25;
};
