// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "PaperSpriteComponent.h"
#include "Net/UnrealNetwork.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	GetRenderComponent()->SetMobility(EComponentMobility::Movable);

	this->SetReplicates(true);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	if (HasAuthority())
	{
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers > 0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			const float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			const float LengthTraveled = (Location - GlobalStartLocation).Size();
			if (LengthTraveled >= JourneyLength)
			{
				const FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}
			Location += Speed * DeltaTime * (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			SetActorLocation(Location);
		}
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}
}

void AMovingPlatform::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMovingPlatform, ActiveTriggers);
}
