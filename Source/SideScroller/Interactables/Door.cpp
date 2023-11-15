// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DoorFlipbook"));
	DoorSprite->SetupAttachment(RootComponent);

	this->InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorInteractBox"));
	this->InteractBox->SetupAttachment(DoorSprite);
	this->InteractBox->SetHiddenInGame(true);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (!DoorSprite)
	{
		return;
	}
	
	this->InteractBox->SetGenerateOverlapEvents(true);
	this->InteractBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBeginOverlapDelegate);
	
	DoorSprite->SetSprite(ClosedPosition);
}

void ADoor::Interact()
{
	ToggleDoor();
}

void ADoor::ToggleDoor()
{
	if (bIsOpen)
	{
		UE_LOG(LogTemp, Display, TEXT("ADoor::ToggleDoor - Setting lever to closed"))
		bIsOpen = false;
		DoorSprite->SetSprite(ClosedPosition);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ADoor::ToggleDoor - Setting lever to open"))
		bIsOpen = true;
		DoorSprite->SetSprite(OpenPosition);
	}
}

void ADoor::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(OtherActor);
	if (PlayerFox == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ADoor::OnBeginOverlapDelegate - Overlap is %s, which is not a PlayerFox"),
			*OtherActor->GetName()
		)
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("ADoor::OnBeginOverlapDelegate - Door Overlapped by %s"),
		*PlayerFox->GetPlayerName().ToString()
	)
	
	PlayerFox->SetInteractableObject(OverlappedComponent);
}

void ADoor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(OtherActor);
	if (PlayerFox == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ADoor::NotifyActorEndOverlap - Overlap was %s, which was not a PlayerFox"),
			*OtherActor->GetName()
		)
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("ADoor::NotifyActorEndOverlap - Door overlap with %s is no longer occuring"),
		*PlayerFox->GetPlayerName().ToString()
	)
	
	PlayerFox->ClearInteractableObject();
}
