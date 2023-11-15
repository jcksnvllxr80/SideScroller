// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ADoor : public APaperSpriteActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ADoor();
	
	UFUNCTION()
	virtual void Interact() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* ClosedPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* OpenPosition;

private:
	UFUNCTION()
	void ToggleDoor();

	UPROPERTY()
	UPaperSpriteComponent* DoorSprite;

	UPROPERTY()
	UBoxComponent* InteractBox;

	UPROPERTY()
	bool bIsOpen = false;

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
