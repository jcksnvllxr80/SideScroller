// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClimbable.h"

#include "BasePaperCharacter.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseClimbable::ABaseClimbable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpriteImage = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteImage"));
	SpriteImage->SetupAttachment(RootComponent);

	this->ClimbableBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ClimbableBox"));
	this->ClimbableBox->SetupAttachment(SpriteImage);
	this->ClimbableBox->SetHiddenInGame(true);
}

UBoxComponent* ABaseClimbable::GetClimbableBox() const
{
	return ClimbableBox;
}

// Called when the game starts or when spawned
void ABaseClimbable::BeginPlay()
{
	Super::BeginPlay();

	this->SpriteImage->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	this->ClimbableBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	this->ClimbableBox->SetGenerateOverlapEvents(true);
	this->ClimbableBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseClimbable::OnBeginOverlapDelegate);
	
}

// Called every frame
void ABaseClimbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseClimbable::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	ABasePaperCharacter* OverlappingActor = dynamic_cast<ABasePaperCharacter*>(OtherComp->GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("%s has overlapped %s!"),
		   *OtherActor->GetName(),
		   *OverlappedComponent->GetOwner()->GetName()
	);
	UGameplayStatics::SpawnSoundAttached(
		this->LadderSound,
		OverlappedComponent,
		TEXT("BasePickupSound")
	);
	// TODO: OverlappingActor->SetNearClimbable(true);
}

