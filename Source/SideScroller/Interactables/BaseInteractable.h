// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "BaseInteractable.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ABaseInteractable : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	ABaseInteractable();

	UFUNCTION()
	bool GetCanInteract() const;

	UFUNCTION()
	void SetCanInteract(const bool CanInteract);
	
private:
	UPROPERTY(replicated)
	bool bCanInteract = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetInteractPrompt;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* InteractPrompt;

protected:
	UPROPERTY(replicated)
	UPaperFlipbookComponent* InteractableFlipbook;

	UPROPERTY()
	UBoxComponent* InteractableBox;
	
	UFUNCTION()
	void OnBeginOverlapDelegate(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UPROPERTY(replicated)
	bool bIsTrue;
	
	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* FalsePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* TruePosition;
	
	virtual void BeginPlay() override;
};
