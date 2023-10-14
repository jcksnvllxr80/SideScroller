// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sidescroller/Characters/BasePaperCharacter.h"
#include "BasePickup.h"
#include "Sidescroller/Interfaces/PickupInterface.h"
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
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) override;

	AMushroom();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float HealingValue = 20.f;
};
