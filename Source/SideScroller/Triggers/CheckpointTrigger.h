// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "CheckpointTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ACheckpointTrigger : public AActor
{
	GENERATED_BODY()

public:
	ACheckpointTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleCheckpoint;

	UPROPERTY(EditAnywhere, Category = Actor)
	UBoxComponent* CheckpointBox;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* CheckpointSound;

private:
	UPROPERTY()
	bool bHasGivenFeedback = false;

	UPROPERTY(VisibleAnywhere, Category = Actor)
	UPaperFlipbookComponent* CheckpointFlipbook;
	
	UFUNCTION(BlueprintCallable, Category = Actor)
	void SpinFlipbook();
	
	UFUNCTION(BlueprintCallable)
	void CheckpointFeedback(UPrimitiveComponent* OverlappedComponent);

protected:
	UFUNCTION()
	void OnBeginOverlapDelegate(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	virtual void BeginPlay() override;
};
