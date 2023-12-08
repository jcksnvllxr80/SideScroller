// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PC_PlayerFox.h"
#include "PC_PlayerGhost.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APC_PlayerGhost : public APC_PlayerFox
{
	GENERATED_BODY()

public:
	APC_PlayerGhost();

protected:
	virtual void BeginPlay() override;
	
};
