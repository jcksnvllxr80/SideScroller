// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "SideScroller/Interfaces/PickupInterface.h"
#include "ExtraLife.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API AExtraLife : public ABasePickup, public IPickupInterface
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) override;

	AExtraLife();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float HealingValue = 20.f;
};