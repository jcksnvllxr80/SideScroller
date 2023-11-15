// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

#include "Components/BoxComponent.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = false;

	LeverSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeverFlipbook"));
	LeverSprite->SetupAttachment(RootComponent);

	this->InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeverInteractBox"));
	this->InteractBox->SetupAttachment(LeverSprite);
	this->InteractBox->SetHiddenInGame(true);
}

void ALever::BeginPlay()
{
	Super::BeginPlay();

	if (!LeverSprite)
	{
		return;
	}
	
	this->InteractBox->SetGenerateOverlapEvents(true);
	this->InteractBox->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnBeginOverlapDelegate);
	
	LeverSprite->SetSprite(OffPosition);
}

void ALever::Interact()
{
	ToggleLever();
}

void ALever::ToggleLever()
{
	if (bIsOn)
	{
		UE_LOG(LogTemp, Display, TEXT("ALever::ToggleLever - Seetting lever to off"));
		bIsOn = false;
		LeverSprite->SetSprite(OffPosition);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ALever::ToggleLever - Seetting lever to on"));
		bIsOn = true;
		LeverSprite->SetSprite(OnPosition);
	}
}

void ALever::OnBeginOverlapDelegate(
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
			TEXT("ALever::OnBeginOverlapDelegate - Overlap is %s, which is not a PlayerFox"),
			*OtherActor->GetName()
		)
		return;
	}

	AActor* ThisActor =  Cast<AActor>(this);
	PlayerFox->SetInteractableObject(ThisActor);
}

void ALever::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(OtherActor);
	if (PlayerFox == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ALever::NotifyActorEndOverlap - Overlap was %s, which was not a PlayerFox"),
			*OtherActor->GetName()
		)
		return;
	}
	
	PlayerFox->SetInteractableObject(nullptr);
}
