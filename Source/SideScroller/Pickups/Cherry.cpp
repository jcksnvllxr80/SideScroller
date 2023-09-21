// Fill out your copyright notice in the Description page of Project Settings.


#include "Cherry.h"

#include "../Characters/Players/PC_PlayerFox.h"

// Sets default values
AACherry::AACherry()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetPickupBox()->SetRelativeScale3D(FVector(0.2,0.2,0.2));
	this->GetPickupBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

// Called when the game starts or when spawned
void AACherry::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AACherry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACherry::GivePickup(APC_PlayerFox* OverlappingActor)
{
	OverlappingActor->TakeCherries(this->CherryAmount);
}
