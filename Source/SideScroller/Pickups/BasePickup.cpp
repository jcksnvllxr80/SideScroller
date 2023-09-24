// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"

#include "../Characters/BasePaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "../Characters/Players/PC_PlayerFox.h"
#include "Engine/DamageEvents.h"
#include "../Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	ABasePaperCharacter* OverlappingActor = dynamic_cast<ABasePaperCharacter*>(OtherComp->GetOwner());
	if (OverlappingActor == nullptr) return;
	
	UE_LOG(LogTemp, Warning, TEXT("%s has overlapped %s!"),
		   *OtherActor->GetName(),
		   *OverlappedComponent->GetOwner()->GetName()
	);
	UGameplayStatics::SpawnSoundAttached(
		this->PickupSound,
		OverlappedComponent,
		TEXT("BasePickupSound")
	);

	if (OverlappedComponent->GetOwner()->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
	{
		Cast<IPickupInterface>(OverlappedComponent->GetOwner())->GivePickup(Cast<APC_PlayerFox>(OverlappingActor));
		OverlappedComponent->GetOwner()->Destroy();
	}
}
