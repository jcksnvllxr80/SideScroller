// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileInterface.generated.h"

/**
 *
 */
UINTERFACE(MinimalAPI)
class UProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @interface IProjectileInterface
 * @brief Interface for handling projectiles in a side-scroller game
 */
class SIDESCROLLER_API IProjectileInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets the transform of the projectile.
	 *
	 * @param Direction The direction in which to launch the projectile.
	 * @param MyOwner The owner of the projectile.
	 * @param BaseChar The base character from which the projectile is fired.
	 * @param Projectile The projectile to set the transform for.
	 */
	UFUNCTION(Category="Projectile")
	virtual void SetProjectileTransform(
		const float Direction,
		AActor* MyOwner,
		const ABasePaperCharacter* BaseChar,
		const ABaseProjectile* Projectile
	) = 0;
};
