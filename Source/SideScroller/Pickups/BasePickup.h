// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sidescroller/Characters/BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

/**
 * \class ABasePickup
 *
 * \brief The ABasePickup class is the base class for all pickups in a side-scroller game.
 *
 * This class defines the basic properties and behaviors of a pickup in a side-scroller game.
 *
 * Inherits from: AActor
 * Dependencies: UPaperFlipbook, UBoxComponent, USoundBase, FTimerHandle, UPrimitiveComponent, AActor, FHitResult
 *
 * \author Your Name
 *
 */
UCLASS()
class SIDESCROLLER_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief IdleAnimation is a variable that holds a reference to a PaperFlipbook asset.
	 *
	 * It is an editable property that can be accessed and modified both in C++ and in Blueprint graphs.
	 * The variable is annotated with the UPROPERTY macro, indicating that it is exposed to the Unreal Editor.
	 * The EditAnywhere specifier allows the variable to be modified in the Details panel of the editor, regardless
	 * of its category. The BlueprintReadWrite specifier indicates that the variable can be both read and written
	 * in Blueprint scripts.
	 *
	 * @see UPaperFlipbook
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleAnimation;

	/**
	 * @brief The ItemTakenAnimation variable holds a reference to a PaperFlipbook asset.
	 *
	 * This variable is used to specify the animation that should play when an item is taken or grabbed by the player.
	 * The animation can be set and modified in the Blueprint editor, as it is exposed for editing anywhere.
	 *
	 * @see UPaperFlipbook, BlueprintReadWrite, EditAnywhere
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* ItemTakenAnimation;

	/**
	 * @brief Constructor for the ABasePickup class.
	 *
	 * This constructor initializes the ABasePickup object by setting up its components and attaching them
	 * appropriately. It also hides the PickupBox component in the game.
	 */
	ABasePickup();

	/**
	 * Updates the state of the object based on the elapsed time since the last frame.
	 *
	 * @param DeltaTime - The time in seconds since the last frame.
	 *
	 * @remark Overrides the Tick method in the AActor class.
	 * @see AActor::Tick
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Get the pickup box component.
	 *
	 * This method returns the `UBoxComponent` that represents the pickup box of the
	 * `ABasePickup` object. The pickup box is a collision component usually used to
	 * detect overlaps with other actors. By calling this method, external classes can
	 * access and manipulate the pickup box component.
	 *
	 * @return A pointer to the `UBoxComponent` representing the pickup box.
	 */
	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetPickupBox() const;

	/**
	 * @brief The sound played when a pickup item is collected.
	 *
	 * This variable represents the sound that will be played when a pickup item is collected in the game.
	 * It is a UPROPERTY with EditAnywhere specifier, allowing it to be edited in the Unreal Engine editor.
	 * The sound is of type USoundBase, which is the base class for all sound assets in Unreal Engine.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	/**
	 * @brief Destroys the actor and clears the timer handle.
	 *
	 * This function destroys the actor by calling the Destroy() method and clears the timer handle used to
	 * track item taken events.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();

private:
	/**
	 * @brief A class member variable that represents a UPaperFlipbookComponent used for displaying a pickup object.
	 *
	 * This variable is accessible from anywhere and is categorized under the "Actor" category.
	 */
	UPROPERTY(VisibleAnywhere, Category = Actor)
	class UPaperFlipbookComponent* PickupFlipbook;

	/**
	 *
	 */
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* PickupBox;

	/**
	 * @brief The time taken for the item taken animation in seconds.
	 *
	 * This variable represents the duration (in seconds) of the animation that plays when an item is taken.
	 * The animation time should be set within the range [0, +infinity] where 0 represents no animation and
	 * larger values represent longer animation durations.
	 *
	 * @note This variable is editable and can be modified through the Unreal Engine Editor.
	 */
	UPROPERTY(EditAnywhere)
	float ItemTakenAnimationTime = 0.5;

	/**
	 *
	 */
	FTimerHandle ItemTakenTimerHandle;

protected:
	/**
	 * Called when this actor begins to overlap another actor.
	 *
	 * @param OverlappedComponent The component that triggered the overlap event.
	 * @param OtherActor The actor that this actor overlapped with.
	 * @param OtherComp The component of the other actor that triggered the overlap event.
	 * @param OtherBodyIndex The index of the body that the other actor has overlapped with.
	 * @param bFromSweep True if this overlap event was caused by a sweep, false otherwise.
	 * @param SweepResult The result of the sweep that caused this overlap event.
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
	 * @brief Called when the object starts playing or is spawned into the level.
	 *
	 * This method is executed when the object starts playing or is spawned into the level.
	 * It performs initialization tasks such as setting up overlap events and assigning animation to the object
	 * if applicable.
	 */
	virtual void BeginPlay() override;
	
};
