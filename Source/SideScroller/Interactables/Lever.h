// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "Lever.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALever : public ABaseInteractable, public IInteractInterface
{
	GENERATED_BODY()

public:
	ALever();
	
	UFUNCTION()
	virtual void Interact() override;

private:
	UFUNCTION()
	void PlayLeverMoveSound() const;

	UPROPERTY(EditAnywhere)
	TArray<class AMovingPlatform*> PlatformsToTrigger;
	
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ToggleLever();

	UFUNCTION()
	void TurnOffLever();

	UFUNCTION()
	void TurnOnLever();

	UFUNCTION()
	void MoveLever();
	
	UPROPERTY(EditAnywhere)
	USoundBase* LeverMoveSound;
	
	FTimerHandle LeverMoveTimerHandle;

	UPROPERTY(EditAnywhere)
	float LeverMoveTime = 0.75;
};
