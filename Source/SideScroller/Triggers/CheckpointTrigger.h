// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "CheckpointTrigger.generated.h"

/**
 * @class ACheckpointTrigger
 * @brief Represents a checkpoint trigger in a side-scrolling game.
 *
 * ACheckpointTrigger is an actor class that triggers checkpoint functionality
 * when the player character overlaps with it. It allows the player to save their
 * progress in the game.
 */
UCLASS()
class SIDESCROLLER_API ACheckpointTrigger : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief A constructor for the ACheckpointTrigger class.
	 *
	 * This constructor initializes the ACheckpointTrigger object and sets up its components.
	 */
	ACheckpointTrigger();

	/**
	 * @brief Updates the checkpoint trigger every frame.
	 *
	 * This method is called every frame and updates the state of the checkpoint trigger. If the checkpoint flipbook
	 * exists and the trigger is set to spin, it will rotate the flipbook based on the CheckPointSpinForce and the
	 * DeltaTime.
	 *
	 * @param DeltaTime The time since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/** The IdleCheckpoint variable is a pointer to a UPaperFlipbook object.
	 *
	 * It is editable anywhere and can be read or modified in Blueprint.
	 *
	 * The UPaperFlipbook class is used to hold and manage 2D sprite animations. The IdleCheckpoint variable points
	 * to a specific sprite animation.
	 *
	 * Example usage:
	 *     IdleCheckpoint->Play();
	 *     IdleCheckpoint->Stop();
	 *     IdleCheckpoint->GetPlaybackPosition();
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleCheckpoint;

	/**
	 * @brief A box component used as a checkpoint.
	 *
	 * The CheckpointBox is an editable box component that can be used as a checkpoint for the actor.
	 * It is typically used to mark a specific location where an actor can return to or trigger a specific action.
	 * The CheckpointBox can be customized in the editor by specifying its size and position.
	 */
	UPROPERTY(EditAnywhere, Category = Actor)
	UBoxComponent* CheckpointBox;

	/**
	 * @brief CheckpointSound is a variable that represents a sound that plays when a checkpoint is reached.
	 *
	 * It is a UPROPERTY that can be edited anywhere and belongs to the Sound category.
	 * It is of type USoundBase*.
	 *
	 */
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* CheckpointSound;

	/**
	 * @brief Represents the spin force applied to a checkpoint actor.
	 *
	 * The CheckPointSpinForce variable stores the magnitude of the spin force to be applied to a checkpoint actor.
	 * It is used in conjunction with the CheckPoint actor class.
	 *
	 * @note The value of the spin force should be set in the editor using the "EditAnywhere" property specifier.
	 * @note The spin force is measured in units per second.
	 */
	UPROPERTY(EditAnywhere, Category = Actor)
	float CheckPointSpinForce = 1000.f;

	/**
	 *
	 */
	UPROPERTY(EditAnywhere, Category = Actor)
	float SpinTime = 1.f;

	/**
	 * @brief Destroys the actor and clears the spin timer.
	 *
	 * This function is a blueprint callable function that destroys the actor
	 * and clears the spin timer associated with it. It also logs a message to
	 * the console indicating the destruction of the actor.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();
	
private:
	/**
	 * @brief Indicates whether the feedback has been given or not.
	 *
	 * This variable is used to track whether the given feedback has been given or not.
	 */
	UPROPERTY()
	bool bHasGivenFeedback = false;

	/**
	 * \class SpinTimerHandle
	 *
	 * \brief Represents a timer handle for a spinning operation.
	 *
	 * This class stores a timer handle used for tracking the duration of a spinning operation.
	 * It allows for starting, stopping, and checking the state of the timer.
	 *
	 * \note This class is part of the spin operation module.
	 */
	FTimerHandle SpinTimerHandle;

	/**
	 * @brief CheckpointFlipbook is a variable representing a UPaperFlipbookComponent used for displaying a flipbook
	 * animation.
	 *
	 * It is a UPROPERTY with the VisibleAnywhere specifier, so it can be seen in the editor. It belongs to the Actor
	 * category.
	 *
	 * @see UPaperFlipbookComponent
	 */
	UPROPERTY(VisibleAnywhere, Category = Actor)
	UPaperFlipbookComponent* CheckpointFlipbook;

	/**
	 * @brief Spins the flipbook.
	 *
	 * This method spins the flipbook by setting the "bSpin" flag to true and scheduling a timer
	 * using the Unreal Engine's timer manager. After the specified SpinTime duration, the method
	 * calls the DestroyActor method to destroy the actor to which this component belongs.
	 *
	 * @note Calling this method will activate the spinning animation and eventually destroy the actor.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor)
	void SpinFlipbook();

	/**
	 * Set the checkpoint locations for all players.
	 *
	 * This method sets the last checkpoint location for all players in the game. It checks if the game mode is of
	 * type ASideScrollerGameModeBase, gets all current players, and updates their last checkpoint location to
	 * the location of this ACheckpointTrigger's CheckpointFlipbook component.
	 *
	 * @see ASideScrollerGameModeBase
	 * @see ACheckpointTrigger::CheckpointFlipbook
	 */
	UFUNCTION(BlueprintCallable, Category = Actor)
	void SetAllPlayersCheckpointLocations() const;

	/**
	 * bool bSpin
	 *
	 * Description: A boolean variable indicating whether the actor is spinning or not.
	 *
	 * Usage:
	 * - Use this variable to control whether the actor should spin or not.
	 * - When set to true, the actor will spin.
	 * - When set to false, the actor will stop spinning.
	 *
	 * Visibility:
	 * - This variable is visible anywhere.
	 *
	 * Category:
	 * - This variable belongs to the "Actor" category.
	 */
	UPROPERTY(VisibleAnywhere, Category = Actor)
	bool bSpin = false;

	/**
	 * @brief CheckpointFeedback function.
	 *
	 * This function is called when the character overlaps with the checkpoint trigger.
	 * It triggers a spin animation on the character and plays a checkpoint sound.
	 *
	 * @param OverlappedComponent The overlapping primitive component.
	 */
	UFUNCTION(BlueprintCallable)
	void CheckpointFeedback(UPrimitiveComponent* OverlappedComponent);

protected:
	/**
	 * Called when this actor begins to overlap another actor.
	 *
	 * @param OverlappedComponent The primitive component that is overlapping another component.
	 * @param OtherActor The actor that is being overlapped by this component.
	 * @param OtherComp The primitive component that is being overlapped by this component.
	 * @param OtherBodyIndex The index of the body that is being overlapped.
	 * @param bFromSweep True if the overlap started due to a sweep, false if it started due to a move.
	 * @param SweepResult Additional information about the sweep that caused the overlap.
	 */
	UFUNCTION()
	void OnBeginOverlapDelegate(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/**
	 * This method is called when the actor is spawned or the level is started.
	 * It sets up the checkpoint flipbook, enables overlap events and adds a dynamic overlap delegate.
	 */
	virtual void BeginPlay() override;
};
