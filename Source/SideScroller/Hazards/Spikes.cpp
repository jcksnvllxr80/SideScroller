// Fill out your copyright notice in the Description page of Project Settings.

#include "Spikes.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "SideScroller/Characters/BasePaperCharacter.h"

/**
 * Constructor for the ASpikes class.
 */
ASpikes::ASpikes()
{
	SpikesSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpikesSprite"));
	SpikesSprite->SetupAttachment(RootComponent);
	
	this->SpikesBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikesInteractBox"));
	this->SpikesBox->SetupAttachment(SpikesSprite);
	this->SpikesBox->SetHiddenInGame(true);

	this->SpikesBox->SetRelativeScale3D(FVector(0.15,0.2,0.02));
	this->SpikesBox->SetRelativeLocation(FVector(-0.500000,0.000000,4.000000));

	this->GetRenderComponent()->CanCharacterStepUpOn = ECB_No;
}

/**
 * @brief Callback function that is invoked when an actor is hit by the spikes.
 *
 * This function is called when the spikes hit an actor. It logs a message with the names of the hit actor and the
 * spikes. If the hit actor is a subclass of ABasePaperCharacter, it calls the TakeDamage function on the hit actor
 * to apply damage.
 *
 * @param HitComponent The primitive component that was hit by the spikes.
 * @param OtherActor The actor that was hit by the spikes.
 * @param OtherComp The primitive component of the actor that was hit by the spikes.
 * @param NormalImpulse The impulse imparted by the spikes on the hit actor.
 * @param Hit The hit result containing information about the hit.
 *
 * @return None
 */
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

/**
 * @brief Called when the game starts or when spawned.
 *
 * This method is called when the game starts or when the ASpikes object is spawned in the world.
 * It is responsible for setting up the collision profiles and adding a dynamic hit event listener.
 */
void ASpikes::BeginPlay()
{
	Super::BeginPlay();

	this->SpikesSprite->SetCollisionProfileName(TEXT("BlockAll"));
	this->SpikesBox->SetCollisionProfileName(TEXT("BlockAll"));
	this->SpikesBox->SetNotifyRigidBodyCollision(true);
	this->SpikesBox->OnComponentHit.AddDynamic(this, &ASpikes::OnHit);
}
