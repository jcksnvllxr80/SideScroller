// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "FallOffLevelTrigger.generated.h"

/**
 * @class AFallOffLevelTrigger
 *
 * @brief Represents a trigger box that detects when an actor overlaps with it, used to handle falling off the level.
 *        Inherits from the ATriggerBox class.
 *
 * @see ATriggerBox
 */
UCLASS()
class SIDESCROLLER_API AFallOffLevelTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	/**
	 * Notifies when an actor begins to overlap with the trigger.
	 *
	 * This method is called when the specified `OtherActor` begins to overlap with the trigger.
	 * It performs additional actions specific to the `FallOffLevelTrigger` class.
	 * If the `OtherActor` is not of type `APC_PlayerFox`, a log message is displayed and the method returns.
	 * Otherwise, a log message is displayed indicating that the `APC_PlayerFox` is overlapping with the trigger,
	 * and the player's `HandleFallOffLevel` method is called.
	 *
	 * @param OtherActor The actor that is beginning to overlap with the trigger.
	 */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
