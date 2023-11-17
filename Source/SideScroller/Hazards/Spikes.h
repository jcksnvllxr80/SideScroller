// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "Spikes.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ASpikes : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	ASpikes();

private:
	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* SpikesSprite;

	UPROPERTY(EditAnywhere)
	UBoxComponent* SpikesBox;

	UPROPERTY(EditAnywhere)
	float DamageValue = 50.0;

protected:
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
	
	UFUNCTION()
	virtual void BeginPlay() override;
};
