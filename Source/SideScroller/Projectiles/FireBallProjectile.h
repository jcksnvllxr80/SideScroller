// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "FireBallProjectile.generated.h"

/**
 * @class AFireBallProjectile
 *
 * @brief A class representing a fire ball projectile in a side-scrolling game.
 *
 * This class inherits from the ABaseProjectile class and provides a specialized implementation for fire
 * ball projectiles.
 *
 * @see ABaseProjectile
 */
UCLASS()
class SIDESCROLLER_API AFireBallProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructs a new instance of AFireBallProjectile.
	 *
	 * @param None
	 *
	 * @return AFireBallProjectile - The newly constructed instance.
	 */
	AFireBallProjectile();

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 *
	 * This method is called when the game starts or when the actor is spawned in the level.
	 */
	virtual void BeginPlay() override;
};
