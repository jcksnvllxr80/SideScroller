// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCollisionPaperCharacter.h"
#include "PaperFlipbook.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"
#include "PC_EnemyFrog.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APC_EnemyFrog : public AEnemyCollisionPaperCharacter, public IProjectileInterface
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* FallAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FrogJumpSound;

	APC_EnemyFrog();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetProjectileTransform(
		const float Direction,
		AActor* MyOwner,
		const ABasePaperCharacter* BaseChar,
		const ABaseProjectile* Projectile
	) override;
	
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	int JumpPeriod = 5;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimation();

protected:
	virtual void Jump() override;
	
	FTimerHandle JumpTimerHandle;
};
