// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "BasePickup.h"
#include "Interfaces/PickupInterface.h"
#include "Mushroom.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API AMushroom : public ABasePickup, public IPickupInterface

{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void GivePickup(ABasePaperCharacter* OverlappingActor) override;

	AMushroom();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	int HealingValue = 20;
};
