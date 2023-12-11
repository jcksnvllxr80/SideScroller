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
	ADoor();
	
	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

	UFUNCTION(BlueprintCallable)
	void CloseDoorSoundAndTimer();

	UFUNCTION(BlueprintCallable)
	void OpenDoorSoundAndTimer();

	UFUNCTION(BlueprintCallable)
	bool GetIsOpen() const;

private:
	UFUNCTION(BlueprintCallable)
	void ToggleDoor();

	UFUNCTION(BlueprintCallable)
	void CloseDoor();
	
	UFUNCTION(BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
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

	UPROPERTY(VisibleAnywhere)
	FVector FlipbookScale;
};
