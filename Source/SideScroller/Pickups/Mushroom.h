// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sidescroller/Characters/BasePaperCharacter.h"
#include "BasePickup.h"
#include "Sidescroller/Interfaces/PickupInterface.h"
#include "Mushroom.generated.h"

/**
 * \class AMushroom
 * \brief A class representing a Mushroom pickup.
 *
 * This class inherits from ABasePickup and IPickupInterface. It provides the basic functionality for a Mushroom
 * pickup, including its ability to heal the player character.
 */
UCLASS()
class SIDESCROLLER_API AMushroom : public ABasePickup, public IPickupInterface

{
	GENERATED_BODY()

public:
	/**
	 * @brief Executes every frame during gameplay.
	 *
	 * This method is called every frame during gameplay to update the state of the Mushroom actor.
	 *
	 * @param DeltaTime The time elapsed since the last frame, in seconds.
	 *
	 * @see Super::Tick()
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * GivePickup
	 *
	 * Gives a pickup to the specified player character by applying a healing value to their health.
	 *
	 * @param OverlappingActor The player character that is overlapping with the pickup.
	 * @return void
	 *
	 * @note This function is a Blueprint callable function, meaning it can be called from Blueprint scripts.
	 * @see APC_PlayerFox
	 */
	UFUNCTION(BlueprintCallable)
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) override;

	/**
	 * @brief Constructor for creating a new instance of the AMushroom class.
	 *
	 * This constructor sets up the AMushroom actor by calling the parent class constructor to enable ticking,
	 * and then modifies the pickup box of the mushroom by setting its relative scale and location.
	 */
	AMushroom();

protected:
	/**
	 * @brief This method is called when the object is first created and begins its play behavior.
	 *
	 * This function is called after the object has been placed in the world or spawned and is ready to begin gameplay.
	 * Any initialization logic should be implemented in this method.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief HealingValue is a float variable that represents the amount of healing provided.
	 *
	 * The HealingValue variable is used to store the amount of healing provided by a specific action or ability.
	 * It is a property which can be edited in the Unreal Engine Editor.
	 * The default value is 20.0.
	 */
	UPROPERTY(EditAnywhere)
	float HealingValue = 20.f;
};
