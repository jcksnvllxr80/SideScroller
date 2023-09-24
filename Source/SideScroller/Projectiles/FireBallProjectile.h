// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "FireBallProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API AFireBallProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	AFireBallProjectile();

protected:
	virtual void BeginPlay() override;
};
