// Fill out your copyright notice in the Description page of Project Settings.


#include "Spikes.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "SideScroller/Characters/BasePaperCharacter.h"

ASpikes::ASpikes()
{
	SpikesSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpikesSprite"));
	SpikesSprite->SetupAttachment(RootComponent);
	
	this->SpikesBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorInteractBox"));
	this->SpikesBox->SetupAttachment(SpikesSprite);
	this->SpikesBox->SetHiddenInGame(true);

	this->SpikesBox->SetRelativeScale3D(FVector(0.15,0.2,0.02));
	this->SpikesBox->SetRelativeLocation(FVector(-0.500000,0.000000,4.000000));

	this->GetRenderComponent()->CanCharacterStepUpOn = ECB_No;
}

void ASpikes::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
) {
	UE_LOG(LogTemp, Verbose, TEXT("ASpikes::OnHit - %s hit %s."), *OtherActor->GetName(), *this->GetName());
	
	ABasePaperCharacter* OtherBasePaperActor = dynamic_cast<ABasePaperCharacter*>(OtherActor);
	if (OtherBasePaperActor == nullptr) return;
	
	OtherBasePaperActor->TakeDamage(
		DamageValue,
		FDamageEvent(UDamageType::StaticClass()),
		this->GetInstigatorController(),
		this
	);

}

void ASpikes::BeginPlay()
{
	Super::BeginPlay();

	this->SpikesSprite->SetCollisionProfileName(TEXT("BlockAll"));
	this->SpikesBox->SetCollisionProfileName(TEXT("BlockAll"));
	this->SpikesBox->SetNotifyRigidBodyCollision(true);
	this->SpikesBox->OnComponentHit.AddDynamic(this, &ASpikes::OnHit);
}
