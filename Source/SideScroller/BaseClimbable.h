// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BaseClimbable.generated.h"

UCLASS()
class SIDESCROLLER_API ABaseClimbable : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* SpriteImage;

	UPROPERTY(EditAnywhere)
	USoundBase* LadderSound;

	// Sets default values for this actor's properties
	ABaseClimbable();

	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetClimbableBox() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void OnBeginOverlapDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* ClimbableBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
