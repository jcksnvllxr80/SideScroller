// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCollisionPaperCharacter.h"
#include "PC_Enemy_Eagle.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APC_Enemy_Eagle : public AEnemyCollisionPaperCharacter
{
	GENERATED_BODY()

	
public:

	APC_Enemy_Eagle();

	virtual void BeginPlay() override;
};
