// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "SideScroller/Interfaces/PickupInterface.h"
#include "ExtraLife.generated.h"

/**
 * @brief Constructor for AExtraLife class.
 *
 * AExtraLife is an actor class that represents an extra life pickup in the game.
 * This constructor initializes the AExtraLife object.
 *
 **/
UCLASS()
class SIDESCROLLER_API AExtraLife : public ABasePickup, public IPickupInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief The Tick method updates the state of the AExtraLife object every frame.
	 *
	 * Tick is a virtual method that is called every frame to update the state of the AExtraLife object.
	 * It overrides the parent class's implementation of the Tick method.
	 *
	 * @param DeltaTime The time in seconds between the current and previous frame.
	 * @return void
	 *
	 * @see Super::Tick(DeltaTime)
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Gives a pickup to the specified player character.
	 *
	 * This method increases the number of lives of the player character by 1.
	 *
	 * @param OverlappingActor The player character to give the pickup to.
	 * @returns None
	 */
	UFUNCTION(BlueprintCallable)
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) override;

	/**
	 * @brief Constructor for AExtraLife class.
	 *
	 * AExtraLife is an actor class that represents an extra life pickup in the game.
	 * This constructor initializes the AExtraLife object.
	 *
	 * @param No parameters required.
	 * @return No return value.
	 */
	AExtraLife();

protected:
	/**
	 * @brief Begins play for the actor.
	 *
	 * This virtual method is called when the actor begins play.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * \brief The amount of healing provided by a healing ability.
	 *
	 * This variable represents the value of healing that a healing ability provides.
	 * The value is a float and can be modified in the editor as it is marked with UPROPERTY(EditAnywhere).
	 * The default value is 20.f.
	 */
	UPROPERTY(EditAnywhere)
	float HealingValue = 20.f;
};
