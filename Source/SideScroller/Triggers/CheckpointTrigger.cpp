// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointTrigger.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/GameModes/SideScrollerGameModeBase.h"

ACheckpointTrigger::ACheckpointTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	CheckpointFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CheckpointPaperFlipbook"));
	CheckpointFlipbook->SetupAttachment(RootComponent);

	this->CheckpointBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckpointBox"));
	this->CheckpointBox->SetupAttachment(CheckpointFlipbook);
	this->CheckpointBox->SetHiddenInGame(true);
	this->CheckpointBox->SetRelativeScale3D(FVector(0.4,0.4,0.4));
	this->CheckpointBox->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

void ACheckpointTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CheckpointFlipbook && this->bSpin)
	{
		CheckpointFlipbook->AddLocalRotation(
			FRotator(0.f, CheckPointSpinForce * DeltaTime, 0.f),
			false,
			nullptr,
			ETeleportType::None
		);
	}
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
	this->bSpin = true;
	GetWorld()->GetTimerManager().SetTimer(
		this->SpinTimerHandle,
		this,
		&ACheckpointTrigger::DestroyActor,
		SpinTime,
		false
	);
}

void ACheckpointTrigger::SetAllPlayersCheckpointLocations() const
{
	const ASideScrollerGameModeBase* GameMode = UECasts_Private::DynamicCast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode()
	);
	if (GameMode != nullptr)
	{
		TArray<APC_PlayerFox*> CurrentPlayers = GameMode->GetPlayers();
		for (APC_PlayerFox* CurrentPlayer : CurrentPlayers)
		{
			CurrentPlayer->SetLastCheckpointLocation(this->CheckpointFlipbook->GetComponentLocation());
		}
	}
}

void ACheckpointTrigger::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	const APC_PlayerFox* Player = dynamic_cast<APC_PlayerFox*>(OtherActor);
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

		SetAllPlayersCheckpointLocations();
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

void ACheckpointTrigger::DestroyActor()
{
	UE_LOG(LogTemp, Display, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->SpinTimerHandle);
}
