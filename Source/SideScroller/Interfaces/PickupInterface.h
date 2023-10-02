// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

class ABasePaperCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIDESCROLLER_API IPickupInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category="Pickup")
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) = 0;
};
