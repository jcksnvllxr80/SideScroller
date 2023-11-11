// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LevelCompleteTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALevelCompleteTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	FVector DoorSize = {8.000000, 20.000000, 16.000000};
};
