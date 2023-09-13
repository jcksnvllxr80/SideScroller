// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "GameFramework/Actor.h"
#include "ACherry.generated.h"

UCLASS()
class SIDESCROLLER_API AACherry : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleAnimation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Sets default values for this actor's properties
	AACherry();

private:
	UPROPERTY(VisibleAnywhere, Category = Actor)
	class UPaperFlipbookComponent* CherryFlipbook;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
