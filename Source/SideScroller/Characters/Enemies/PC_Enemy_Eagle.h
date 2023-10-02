// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCollisionPaperCharacter.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"
#include "PC_Enemy_Eagle.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APC_Enemy_Eagle : public AEnemyCollisionPaperCharacter, public IProjectileInterface
{
	GENERATED_BODY()
	
public:

	APC_Enemy_Eagle();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetProjectileTransform(
		const float Direction,
		AActor* MyOwner,
		const ABasePaperCharacter* BaseChar,
		const ABaseProjectile* Projectile
	) override;

private:
	UFUNCTION(BlueprintCallable)
	float GetEnemyToPlayerPitchRadians(
		const ABasePaperCharacter* BaseChar,
		FRotator& OwnerRotation,
		float Direction
	) const;
};
