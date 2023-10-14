// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointTrigger.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

ACheckpointTrigger::ACheckpointTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	CheckpointFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CheckpointPaperFlipbook"));
	CheckpointFlipbook->SetupAttachment(RootComponent);

	this->CheckpointBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckpointBox"));
	this->CheckpointBox->SetupAttachment(CheckpointFlipbook);
	this->CheckpointBox->SetHiddenInGame(true);
	this->CheckpointBox->SetRelativeScale3D(FVector(0.4,0.4,0.4));
	this->CheckpointBox->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

void ACheckpointTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (!CheckpointFlipbook) return;
	
	this->CheckpointFlipbook->SetFlipbook(IdleCheckpoint);
	
	this->CheckpointBox->SetGenerateOverlapEvents(true);
	this->CheckpointBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointTrigger::OnBeginOverlapDelegate);
}

void ACheckpointTrigger::SpinFlipbook()
{
	// TODO: get the checkpoint spinning
	CheckpointFlipbook->AddRadialImpulse(
		GetActorLocation(),
		10.f,
		1000.f,
		RIF_Linear,
		true
	);
}

void ACheckpointTrigger::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Overlap of CheckpointTrigger is not PC_PlayerFox."));
		return;
	}
	
	if (!bHasGivenFeedback)
	{
		this->bHasGivenFeedback = true;
		CheckpointFeedback(OverlappedComponent);
		UE_LOG(LogTemp, Display, TEXT("PC_PlayerFox, %s, overlapping CheckpointTrigger."), *Player->GetName());
		Player->SetCheckpointLocation(this->CheckpointFlipbook->GetComponentLocation());
	}
}

void ACheckpointTrigger::CheckpointFeedback(UPrimitiveComponent* OverlappedComponent)
{
	SpinFlipbook();
	
	UGameplayStatics::SpawnSoundAttached(
		this->CheckpointSound,
		OverlappedComponent,
		TEXT("BasePickupSound")
	);
}
