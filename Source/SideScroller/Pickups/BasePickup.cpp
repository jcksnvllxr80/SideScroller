// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"

#include "Sidescroller/Characters/BasePaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Sidescroller/Characters/Players/PC_PlayerFox.h"
#include "Engine/DamageEvents.h"
#include "Sidescroller/Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

/**
 * ABasePickup constructor.
 *
 * It initializes the ABasePickup object by setting up the actor tick, creating a default subobject of
 * UPaperFlipbookComponent with the name "PickupFlipbook", and attaching it to the root component. It also
 * creates a default subobject of UBoxComponent with the name "PickupBox", attaches it to the PickupFlipbook
 * component, and sets it to be hidden in the game.
 */
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PickupFlipbook"));
	PickupFlipbook->SetupAttachment(RootComponent);

	this->PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBox"));
	this->PickupBox->SetupAttachment(PickupFlipbook);
	this->PickupBox->SetHiddenInGame(true);
}


/**
 * Begins play for the pickup object.
 *
 * @param None.
 * @return None.
 */
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();

	if (!PickupFlipbook)
	{
		return;
	}
	
	this->PickupBox->SetGenerateOverlapEvents(true);
	this->PickupBox->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnBeginOverlapDelegate);
	
	PickupFlipbook->SetFlipbook(IdleAnimation);
}

/**
 * @brief Updates the pickup's state every frame.
 *
 * This method is called every frame to update the pickup's state based on the elapsed time.
 *
 * @param DeltaTime The time in seconds since the last frame.
 */
void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * @brief Get the pickup box component.
 *
 * This method returns the UBoxComponent used for collision detection by the pickup object.
 *
 * @return A pointer to the UBoxComponent used as the pickup box.
 */
UBoxComponent* ABasePickup::GetPickupBox() const
{
	return PickupBox;
}

/**
 * Called when this pickup begins overlapping with another actor.
 *
 * @param OverlappedComponent The primitive component of this pickup that has overlapped with another component.
 * @param OtherActor The actor that this pickup has overlapped with.
 * @param OtherComp The primitive component of the other actor that this pickup has overlapped with.
 * @param OtherBodyIndex The body index of the other actor that this pickup has overlapped with.
 * @param bFromSweep Indicates whether the overlap was caused by a sweep or not.
 * @param SweepResult The hit result of the sweep that caused the overlap.
 */
void ABasePickup::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	UE_LOG(LogTemp, Display, TEXT("%s has overlapped %s!"),
		*OtherActor->GetName(),
		*OverlappedComponent->GetOwner()->GetName()
	);

	APC_PlayerFox* OverlappingActor = dynamic_cast<APC_PlayerFox*>(OtherComp->GetOwner());
	if (OverlappingActor == nullptr) return;

	UGameplayStatics::SpawnSoundAttached(
		this->PickupSound,
		OverlappedComponent,
		TEXT("BasePickupSound")
	);

	// dont allow this pickup to be taken more than once
	this->PickupBox->SetGenerateOverlapEvents(false);
	PickupFlipbook->SetFlipbook(ItemTakenAnimation);
	GetWorld()->GetTimerManager().SetTimer(
		this->ItemTakenTimerHandle,
		this,
		&ABasePickup::DestroyActor,
		ItemTakenAnimationTime,
		false
	);

	if (OverlappedComponent->GetOwner()->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
	{
		Cast<IPickupInterface>(OverlappedComponent->GetOwner())->GivePickup(OverlappingActor);
	}
}

/**
 * @brief Destroys the actor and clears the timer for the item taken.
 *
 * This method destroys the actor and clears the timer handle for the item taken. It logs a verbose message indicating
 * the name of the actor being destroyed.
 *
 * @note The actor being destroyed must be derived from the ABasePickup class.
 *
 * @see ABasePickup
 */
void ABasePickup::DestroyActor()
{
	UE_LOG(LogTemp, Verbose, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->ItemTakenTimerHandle);
}