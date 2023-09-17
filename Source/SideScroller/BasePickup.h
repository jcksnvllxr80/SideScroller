// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

UCLASS()
class SIDESCROLLER_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleAnimation;
	
	// Sets default values for this actor's properties
	ABasePickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetPickupBox() const;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

private:
	UPROPERTY(VisibleAnywhere, Category = Actor)
	class UPaperFlipbookComponent* PickupFlipbook;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* PickupBox;

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
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
