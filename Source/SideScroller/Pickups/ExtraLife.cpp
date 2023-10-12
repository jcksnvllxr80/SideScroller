// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtraLife.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"

// Sets default values
AExtraLife::AExtraLife()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->GetPickupBox()->SetRelativeScale3D(FVector(0.2,0.2,0.2));
	this->GetPickupBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

// Called when the game starts or when spawned
void AExtraLife::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AExtraLife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExtraLife::GivePickup(APC_PlayerFox* OverlappingActor)
{
	OverlappingActor->SetNumberOfLives(OverlappingActor->GetNumberOfLives() + 1);
}
