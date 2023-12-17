// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClimbable.h"

#include "PaperSpriteComponent.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "Components/BoxComponent.h"

/**
 * Constructor for ABaseClimbable class.
 */
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

/**
 * @brief Function called when the object begins play.
 *
 * This function is automatically called when the object is spawned or added to the world.
 */
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

/**
 * @brief This method is called every frame during gameplay to update the climbable object.
 *
 * @param DeltaTime The time passed since the last frame, in seconds.
 */
void ABaseClimbable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * Called when an overlap event begins.
 *
 * This method is triggered when an overlapped component starts overlapping with another component in the scene.
 * The method performs the following actions:
 * - Retrieves the owning ABaseClimbable instance from the OverlappedComponent.
 * - Retrieves the owning APC_PlayerFox instance from the OtherComp component.
 * - Checks if the OverlappingActor is null. If so, the method returns without performing any further actions.
 * - Logs a message to the LogTemp with information about the overlapped actors.
 * - Calls the SetOverlappingClimbable method on the OverlappingActor, passing the OverlappedClimbableComponent.
 *
 * @param OverlappedComponent  The primitive component that triggered the overlap event.
 * @param OtherActor           The other actor involved in the overlap event.
 * @param OtherComp            The primitive component involved in the overlap event.
 * @param OtherBodyIndex       The body index of the other component involved in the overlap event.
 * @param bFromSweep           Indicates whether the overlap event was caused by a sweep.
 * @param SweepResult          Additional information about the sweep collision.
 */
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
	
	UE_LOG(LogTemp, Verbose, TEXT("%s has overlapped %s!"),
		   *OverlappingActor->GetName(),
		   *OverlappedComponent->GetOwner()->GetName()
	);
	OverlappingActor->SetOverlappingClimbable(true, OverlappedClimbableComponent);
}

/**
 * Callback function for when an overlap ends between this climbable object and another actor.
 *
 * @param OverlappedComponent The component that this object overlapped with.
 * @param OtherActor The actor that this object overlapped with.
 * @param OtherComp The component of the other actor that this object overlapped with.
 * @param OtherBodyIndex The body index of the other actor that this object overlapped with.
 */
void ABaseClimbable::OnEndOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) {
	ABaseClimbable* OverlappedClimbableComponent = dynamic_cast<ABaseClimbable*>(OverlappedComponent->GetOwner());
	APC_PlayerFox* OverlappingActor = dynamic_cast<APC_PlayerFox*>(OtherComp->GetOwner());
	if (OverlappingActor == nullptr) return;
	
	UE_LOG(LogTemp, Verbose, TEXT("%s is no longer overlapping %s!"),
	   *OverlappingActor->GetName(),
	   *OverlappedComponent->GetOwner()->GetName()
	);
	OverlappingActor->SetOverlappingClimbable(false, OverlappedClimbableComponent);
}

