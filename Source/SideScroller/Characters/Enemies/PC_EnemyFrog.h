// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCollisionPaperCharacter.h"
#include "PaperFlipbook.h"
#include "PC_EnemyFrog.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APC_EnemyFrog : public AEnemyCollisionPaperCharacter
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* JumpAnimation;

	APC_EnemyFrog();

	virtual void BeginPlay() override;
};
