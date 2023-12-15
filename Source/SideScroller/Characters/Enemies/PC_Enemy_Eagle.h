// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCollisionPaperCharacter.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"
#include "PC_Enemy_Eagle.generated.h"

/**
 * @class APC_Enemy_Eagle
 *
 * @brief This class represents the enemy eagle character in the side scroller game.
 *
 * The APC_Enemy_Eagle class is a derived class of AEnemyCollisionPaperCharacter and implements
 * the IProjectileInterface. It represents the enemy eagle character in the side scroller game.
 *
 * The enemy eagle character has the ability to set the projectile transform, which determines
 * the direction of the projectile. It also has a private utility function to calculate the
 * enemy to player pitch in radians.
 */
UCLASS()
class SIDESCROLLER_API APC_Enemy_Eagle : public AEnemyCollisionPaperCharacter, public IProjectileInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Default constructor for APC_Enemy_Eagle class.
	 *
	 * Initializes the initial values for the APC_Enemy_Eagle object.
	 * Sets the tick option to false for the primary actor.
	 * Sets the relative location and scale for the damage box, left hurt box, and right hurt box.
	 * Enables flying for the character movement.
	 * Sets the damage value to 20.0.
	 *
	 * @param None
	 * @return None
	 */
	APC_Enemy_Eagle();

	/**
	 * Begins playing the game.
	 *
	 * This method is called when the game starts and is used to initialize the game state or perform any necessary
	 * setup. It is often used to set initial parameters and behaviors for the character or actor.
	 *
	 * It is important to call the parent class's `BeginPlay` method using `Super::BeginPlay()` to ensure any
	 * necessary parent initialization is performed.
	 *
	 * After calling the parent class's `BeginPlay` method, this method sets the movement mode of the character to
	 * `MOVE_Flying` and sets the maximum fly speed to 50 units per second.
	 */
	virtual void BeginPlay() override;

	/**
	 * Sets the transform of a projectile based on the given parameters.
	 *
	 * @param Direction The direction of the projectile.
	 * @param MyOwner The owner of the projectile.
	 * @param BaseChar The base character associated with the projectile.
	 * @param Projectile The projectile to set the transform for.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetProjectileTransform(
		const float Direction,
		AActor* MyOwner,
		const ABasePaperCharacter* BaseChar,
		const ABaseProjectile* Projectile
	) override;

private:
	/**
	 * Calculates the pitch in radians from the enemy character to the player character.
	 *
	 * @param BaseChar The enemy character.
	 * @param OwnerRotation The rotation of the enemy character.
	 * @param Direction The direction of the pitch.
	 * @return The pitch in radians from the enemy character to the player character.
	 */
	UFUNCTION(BlueprintCallable)
	float GetEnemyToPlayerPitchRadians(
		const ABasePaperCharacter* BaseChar,
		FRotator& OwnerRotation,
		float Direction
	) const;
};
