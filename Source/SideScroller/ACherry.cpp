// Fill out your copyright notice in the Description page of Project Settings.


#include "ACherry.h"

#include "PaperFlipbookComponent.h"

// Sets default values
AACherry::AACherry()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CherryFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CherryFlipbook"));
	CherryFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AACherry::BeginPlay()
{
	Super::BeginPlay();
	if (CherryFlipbook)
	{
		CherryFlipbook->SetFlipbook(IdleAnimation);
	} else {
		return;
	}
}

// Called every frame
void AACherry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

