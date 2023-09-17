// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "Mushroom.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API AMushroom : public ABasePickup
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	AMushroom();

protected:
	virtual void BeginPlay() override;
};
