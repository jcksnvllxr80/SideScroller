// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "Lever.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ALever : public APaperSpriteActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ALever();
	
	UFUNCTION()
	virtual void Interact() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* OffPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* OnPosition;

private:
	UFUNCTION()
	void ToggleLever();

	UPROPERTY()
	UPaperSpriteComponent* LeverSprite;

	UPROPERTY()
	UBoxComponent* InteractBox;
	
	UPROPERTY()
	bool bIsOn = false;

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

	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void BeginPlay() override;
};
