// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PC_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APC_AIController : public AAIController
{
	GENERATED_BODY()

public:
	APC_AIController();

	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();

	void CanShootAgain();
	
	virtual void Tick(float DeltaSeconds) override;	
	
private:
	APawn* PlayerPawn = nullptr;
	
	FTimerHandle ShootTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float ShootDelayTime = 2.0;
	
	bool CanShoot = true;

protected:
	virtual void BeginPlay() override;
};
