// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "BaseInteractable.generated.h"

/**
 * @class ABaseInteractable
 *
 * @brief Base class for interactable actors in a side-scrolling game.
 *
 * The ABaseInteractable class is derived from APaperSpriteActor and provides functionality for interactable actors
 * in a side-scrolling game. It contains methods to get and set the ability to interact, as well as properties for
 * interact prompt and flipbook components.
 *
 * @see APaperSpriteActor
 */
UCLASS()
class SIDESCROLLER_API ABaseInteractable : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for the base interactable class.
	 * @param None
	 * @return None
	 */
	ABaseInteractable();

	/**
	 * \brief Returns whether the interactable can be interacted with or not.
	 *
	 * \return True if the interactable can be interacted with, false otherwise.
	 */
	UFUNCTION()
	bool GetCanInteract() const;

	/**
	 * Sets the value of the CanInteract property.
	 *
	 * @param CanInteract The new value for the CanInteract property.
	 */
	UFUNCTION()
	void SetCanInteract(const bool CanInteract);
	
private:
	/**
	 * @brief Determines if the actor can be interacted with by the player.
	 *
	 * When this property is set to true, the actor can be interacted with by the player.
	 * The interaction could be anything like picking up an item, opening a door, or triggering a specific action.
	 *
	 * @note This property is replicated, meaning its value is synchronized across the network if this actor is
	 * replicated.
	 *
	 * @see AActor
	 */
	UPROPERTY(replicated)
	bool bCanInteract = true;

	/**
	 * @brief The WidgetInteractPrompt variable is an editable property that holds a subclass of UUserWidget.
	 *
	 * UPROPERTY macro is used to declare a variable that can be edited in the editor. EditAnywhere flag allows
	 * the variable to be edited anywhere, which means it can be changed in the editor or through Blueprint scripting.
	 * The variable represents a subclass of UUserWidget, which is a base class for user interface widgets.
	 *
	 * This variable can be used to associate a specific user widget with the Interactable object in the game. The
	 * user widget can be displayed when the player interacts with the object. The interaction details can be
	 * customized using the properties of the assigned user widget subclass.
	 *
	 * @see UPROPERTY, UUserWidget
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetInteractPrompt;

	/**
	 * @brief The InteractPrompt variable represents a UWidgetComponent that is used for displaying prompts or
	 * interactive elements in the game.
	 *
	 * The InteractPrompt variable is an EditAnywhere UPROPERTY that allows designers to assign a UWidgetComponent
	 * to be used as the interact prompt in the game. This UWidgetComponent can be used to display prompts or
	 * interactive elements to the player that may vary based on the current game state or context.
	 *
	 * Usage:
	 * - Designers can assign a UWidgetComponent to the InteractPrompt variable through Unreal Editor's Details Panel.
	 * - The assigned UWidgetComponent will be used to display prompts or interactive elements to the player in
	 * the game.
	 *
	 * Remarks:
	 * - The InteractPrompt variable can be nullptr if no UWidgetComponent is assigned to it.
	 * - Its value can be accessed and modified during runtime to update the interact prompt in real-time.
	 */
	UPROPERTY(EditAnywhere)
	UWidgetComponent* InteractPrompt;

protected:
	/**
	 * Represents a replicated PaperFlipbookComponent used for interactable objects.
	 */
	UPROPERTY(replicated)
	UPaperFlipbookComponent* InteractableFlipbook;

	/**
	 *
	 */
	UPROPERTY()
	UBoxComponent* InteractableBox;

	/**
	 * Called when this interactable object begins overlapping with another component.
	 *
	 * @param OverlappedComponent The primitive component that this object is overlapping with.
	 * @param OtherActor The actor that is overlapping with this object.
	 * @param OtherComp The primitive component of the other actor that is overlapping with this object.
	 * @param OtherBodyIndex The index of the body that is overlapping with this object.
	 * @param bFromSweep Whether the overlap is from a sweep.
	 * @param SweepResult The hit result of the overlap sweep.
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
	 * @brief Variable to hold a boolean value indicating whether an action is true or not.
	 *
	 * This variable is replicated, meaning it will be synchronized across the network for multiplayer games.
	 * The value of this variable determines the state of an action - true for active or completed, false for
	 * inactive or incomplete.
	 */
	UPROPERTY(replicated)
	bool bIsTrue;

	/**
	 * @brief Called when an actor stops overlapping with this interactable object.
	 *
	 * @param OtherActor The actor that is no longer overlapping with this interactable object.
	 *
	 * This method is called when an actor stops overlapping with this interactable object.
	 * It performs the following actions:
	 * 1. Calls the parent class implementation of NotifyActorEndOverlap.
	 * 2. Checks if the OtherActor is of type APC_PlayerFox. If not, it logs an error message and returns.
	 * 3. Logs a message indicating that the overlap with the player is no longer occurring.
	 * 4. Checks if the InteractPrompt widget is valid and hides it if so.
	 * 5. Clears the interactable object from the player character.
	 *
	 * @note This method is typically called automatically by the Unreal Engine when an actor stops
	 * overlapping with another actor. It should not be called manually.
	 */
	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	/**
	 * @brief The FalsePosition variable represents a reference to a UPaperFlipbook object.
	 *
	 * This variable is editable anywhere and blueprint readable/writeable.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* FalsePosition;

	/**
	 * @brief TruePosition is a pointer to a UPaperFlipbook object that represents the true position of something.
	 *
	 * The TruePosition variable can be edited anywhere and read from blueprints. It is used to store and manipulate
	 * a flipbook object that contains the true position information.
	 *
	 * This variable is of type UPaperFlipbook*, which is a pointer to a UPaperFlipbook object. UPaperFlipbook
	 * is a class in Unreal Engine that represents a 2D animation stored as a series of frames.
	 *
	 * Example usage:
	 *     TruePosition = MyTruePositionFlipbook;
	 *
	 * @see UPaperFlipbook
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* TruePosition;

	/**
	 * @brief Called when the object begins to play in the game world.
	 *
	 * This function is called when the Actor is spawned into the game world or when the game begins.
	 * It is used to initialize any necessary variables or perform any setup actions before the object
	 * starts interacting with the game world.
	 *
	 * @param None
	 *
	 * @return None
	 */
	virtual void BeginPlay() override;
};
