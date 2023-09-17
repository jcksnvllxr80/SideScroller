// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/PickupInterface.h"
#include "Cherry.generated.h"

UCLASS()
class SIDESCROLLER_API AACherry : public ABasePickup, public IPickupInterface
{
	GENERATED_BODY()
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) override;

	// Sets default values for this actor's properties
	AACherry();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int CherryAmount = 2;
};
