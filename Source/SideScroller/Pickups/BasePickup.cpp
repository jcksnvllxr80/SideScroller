// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"

#include "Sidescroller/Characters/BasePaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Sidescroller/Characters/Players/PC_PlayerFox.h"
#include "Engine/DamageEvents.h"
#include "Sidescroller/Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
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


// Called when the game starts or when spawned
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

// Called every frame
void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBoxComponent* ABasePickup::GetPickupBox() const
{
	return PickupBox;
}

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

void ABasePickup::DestroyActor()
{
	UE_LOG(LogTemp, Verbose, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->ItemTakenTimerHandle);
}