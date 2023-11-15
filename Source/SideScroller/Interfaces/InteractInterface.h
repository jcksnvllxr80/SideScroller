// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class ABasePaperCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIDESCROLLER_API IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category="Interact")
	virtual void Interact() = 0;
};