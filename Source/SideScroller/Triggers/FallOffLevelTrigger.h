// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "FallOffLevelTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API AFallOffLevelTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
