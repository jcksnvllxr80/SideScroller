// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

/**
 * \class APlatformTrigger
 *
 * \brief A class representing a platform trigger in a side-scrolling game.
 *
 * APlatformTrigger is an actor that detects when another actor overlaps with its trigger volume. When the
 * overlap begins, the platform trigger activates a set of moving platforms. When the overlap ends, the platform
 * trigger deactivates the moving platforms.
 */
UCLASS()
class SIDESCROLLER_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Constructs an instance of APlatformTrigger.
	 *
	 * This method initializes the APlatformTrigger object. It sets up the actor to call Tick() every frame and
	 * assigns a UBoxComponent to the TriggerVolume variable. It also sets TriggerVolume as the root component of
	 * the actor. Additionally, it binds the OnComponentBeginOverlap and OnComponentEndOverlap events to the
	 * APlatformTrigger::OnOverlapBegin and APlatformTrigger::OnOverlapEnd methods respectively.
	 *
	 * @return None.
	 */
	APlatformTrigger();

protected:
	/**
	 * \brief Called when the game starts or when spawned.
	 *
	 * This is a virtual method that is called when the game starts or when the object is spawned.
	 * Override this method in derived classes to implement additional functionality.
	 *
	 * \details This method should be used for initialization and setup tasks that need to be performed
	 * when the object is first created. For example, initializing variables, setting up references to
	 * other objects, or registering event listeners.
	 *
	 * \note This method should be called through the "Super::" keyword to invoke the parent class implementation
	 * of this method before any additional functionality is added.
	 *
	 * \param None
	 *
	 * \return None
	 *
	 * \see Super::BeginPlay()
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * @brief Tick function called every frame.
	 * @param DeltaTime The time passed since the last frame.
	 *
	 * This function is called every frame to update the object's state.
	 * It is a virtual function and can be overridden by derived classes to provide custom behavior.
	 * The DeltaTime parameter represents the time passed since the last frame, allowing for time-based
	 * calculations. In this specific implementation, the Super::Tick(DeltaTime) function is called to invoke
	 * the base class's tick function. This ensures that any necessary base class functionality is also
	 * executed during the tick operation.
	 */
	virtual void Tick(float DeltaTime) override;

private:
	/**
	 * The TriggerVolume variable represents a UBoxComponent that is visible anywhere.
	 */
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerVolume;

	/**
	 * @brief Array of AMovingPlatform objects that can be triggered.
	 *
	 * This array holds references to AMovingPlatform objects that can be triggered. These objects are of
	 * type AMovingPlatform*. The array is editable anywhere and can store multiple objects.
	 */
	UPROPERTY(EditAnywhere)
	TArray<class AMovingPlatform*> PlatformsToTrigger;

	/**
	 * Called when an overlap begins between this actor and another actor or component.
	 *
	 * @param OverlappedComp The primitive component that has been overlapped.
	 * @param OtherActor The actor that has overlapped with this component.
	 * @param OtherComp The primitive component that has overlapped with this component.
	 * @param OtherBodyIndex The index of the body that has overlapped with this component.
	 * @param bFromSweep True if the overlap was initiated as a result of a sweep, false otherwise.
	 * @param SweepResult The result of the sweep that caused the overlap.
	 */
	UFUNCTION()
	void OnOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/**
	 * Method called when an overlap ends between the provided components.
	 *
	 * @param OverlappedComp The primitive component that was overlapped.
	 * @param OtherActor The actor that overlapped the component.
	 * @param OtherComp The primitive component that overlapped the specified component.
	 * @param OtherBodyIndex The index of the body that the other component has in the physics system.
	 */
	UFUNCTION()
	void OnOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};
