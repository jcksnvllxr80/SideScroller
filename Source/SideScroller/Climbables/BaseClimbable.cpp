// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClimbable.h"

#include "../Characters/BasePaperCharacter.h"
#include "PaperSpriteComponent.h"
#include "../Characters/Players/PC_PlayerFox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseClimbable::ABaseClimbable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	// this->SpriteImage->SetGenerateOverlapEvents(true);
	this->ClimbableBox->SetGenerateOverlapEvents(true);
	// this->SpriteImage->OnComponentBeginOverlap.AddDynamic(this, &ABaseClimbable::OnBeginOverlapDelegate);
	this->ClimbableBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseClimbable::OnBeginOverlapDelegate);
	this->ClimbableBox->OnComponentEndOverlap.AddDynamic(this, &ABaseClimbable::OnEndOverlapDelegate);
	
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
	ABaseClimbable* OverlappedClimbableComponent = dynamic_cast<ABaseClimbable*>(OverlappedComponent->GetOwner());
	APC_PlayerFox* OverlappingActor = dynamic_cast<APC_PlayerFox*>(OtherComp->GetOwner());
	if (OverlappingActor == nullptr) return;
	
	UE_LOG(LogTemp, Warning, TEXT("%s has overlapped %s!"),
		   *OverlappingActor->GetName(),
		   *OverlappedComponent->GetOwner()->GetName()
	);
	OverlappingActor->SetOverlappingClimbable(true, OverlappedClimbableComponent);
}

void ABaseClimbable::OnEndOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) {
	ABaseClimbable* OverlappedClimbableComponent = dynamic_cast<ABaseClimbable*>(OverlappedComponent->GetOwner());
	APC_PlayerFox* OverlappingActor = dynamic_cast<APC_PlayerFox*>(OtherComp->GetOwner());
	if (OverlappingActor == nullptr) return;
	
	UE_LOG(LogTemp, Warning, TEXT("%s is no longer overlapping %s!"),
	   *OverlappingActor->GetName(),
	   *OverlappedComponent->GetOwner()->GetName()
);
	OverlappingActor->SetOverlappingClimbable(false, OverlappedClimbableComponent);
}

