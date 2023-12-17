// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "GameFramework/Actor.h"
#include "Sidescroller/Interfaces/PickupInterface.h"
#include "Cherry.generated.h"

/**
 * \class AACherry
 * \brief Represents a cherry pickup in the game.
 *
 * This class derives from ABasePickup and implements the IPickupInterface.
 * It provides functionality for giving the pickup to the player when they overlap with it.
 */
UCLASS()
class SIDESCROLLER_API AACherry : public ABasePickup, public IPickupInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * \brief This method is called every frame to update the state of the actor.
	 * \param DeltaTime The time since the last frame in seconds.
	 *
	 * This method is overridden from the parent class and is used to update the state
	 * of the AACherry actor every frame. It is called automatically by the engine and
	 * should not be called manually.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Gives a pickup to the specified player character.
	 *
	 * @param OverlappingActor - The player character that is overlapping with the pickup.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) override;

	/**
	 * @brief Constructor for the AACherry class.
	 *
	 * Initializes the AACherry actor by setting its properties and
	 * configuring its components.
	 */
	AACherry();
	
protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 *
	 * This method is called when the game starts or when the object is spawned in the level.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief The number of cherries available.
	 *
	 * This variable represents the amount of cherries in a given context. It is
	 * an integer value, and can be modified using the "EditAnywhere" property
	 * specifier.
	 */
	UPROPERTY(EditAnywhere)
	int CherryAmount = 2;
};
