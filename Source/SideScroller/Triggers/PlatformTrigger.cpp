// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "SideScroller/Mechanics/PlatformBlocks/MovingPlatform.h"

/**
 * APlatformTrigger constructor
 *
 * This method initializes the APlatformTrigger object with default values. It creates a trigger volume and
 * sets it as the root component of the actor. It also sets up the event bindings for when an overlap
 * begins and ends.
 */
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("TriggerVolume")));
	if (!TriggerVolume) return;
	
	RootComponent = TriggerVolume;

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

/**
 * Called when the game starts or when spawned.
 *
 * @param None
 * @return None
 */
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * @brief Executes each frame during gameplay.
 *
 * This method is called every frame to update the state of the APlatformTrigger object. It inherits from the
 * Super class's Tick method, which executes the same method in the parent class. The APlatformTrigger::Tick
 * method takes a float parameter named DeltaTime, which represents the time in seconds since the last frame.
 *
 * @param DeltaTime The time elapsed in seconds since the last frame.
 */
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * Function called when an overlap begins.
 *
 * @param OverlappedComp The overlapping primitive component.
 * @param OtherActor The actor that is overlapping.
 * @param OtherComp The primitive component that is overlapping.
 * @param OtherBodyIndex The index of the body that is overlapping.
 * @param bFromSweep If the overlap was caused by the component sweeping.
 * @param SweepResult The result of the sweep test that caused the overlap.
 */
void APlatformTrigger::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	UE_LOG(LogTemp, Display, TEXT("An Object has activated the platform trigger."))
	for (AMovingPlatform* Platform: PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

/**
 * @fn void APlatformTrigger::OnOverlapEnd(
 *		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
 * )
 * @brief This method is called when an overlap with the trigger ends.
 *
 * @param OverlappedComp The primitive component that triggered the overlap.
 * @param OtherActor The actor that ended the overlap.
 * @param OtherComp The primitive component of the actor that ended the overlap.
 * @param OtherBodyIndex The index of the overlapping body of the actor.
 *
 * This method is responsible for handling the end of an overlap with the trigger. When an object leaves the
 * trigger, it will print a log message indicating that the object has left causing the trigger to be deactivated.
 * Additionally, all the platforms associated with this trigger will have their active trigger removed.
 */
void APlatformTrigger::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) {
	UE_LOG(LogTemp, Display, TEXT("An object has left the platform trigger causing it to be decativated."))
	for (AMovingPlatform* Platform: PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}

