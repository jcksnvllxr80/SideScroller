// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BaseClimbable.generated.h"

/**
 * @class ABaseClimbable
 * @brief Represents a climbable object in a side-scroller game.
 *        This class inherits from AActor, the base class for all actors in Unreal Engine.
 */
UCLASS()
class SIDESCROLLER_API ABaseClimbable : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief A variable representing a sprite image component.
	 *
	 * This variable is used to store and manipulate a UPaperSpriteComponent object, which represents a 2D sprite image
	 * component in the game. The UPaperSpriteComponent class is part of the Unreal Engine and is used for rendering
	 * sprite images in a game world.
	 *
	 * You can use this variable to access and modify properties of the sprite image component, such as its material,
	 * visibility, and size.
	 *
	 * This variable is marked with the EditAnywhere and BlueprintReadWrite UPROPERTY specifiers, which means that
	 * it can be modified both in the editor and in blueprint scripts.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* SpriteImage;

	/**
	 * @brief Represents the ladder sound effect.
	 *
	 * This variable is used to store the ladder sound effect associated with an object.
	 * It is of type USoundBase* which refers to a sound asset in Unreal Engine.
	 * The sound effect can be edited from anywhere, making it accessible in the Unreal Editor.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* LadderSound;

	/**
	 * @brief Default constructor for ABaseClimbable class.
	 *
	 * This constructor initializes the ABaseClimbable object and sets up the necessary components.
	 * It sets the actor to call Tick() every frame and attaches a sprite component and a box component.
	 * The box component is hidden in the game.
	 */
	ABaseClimbable();

	/**
	 * \brief Retrieves the climbable box component.
	 *
	 * \return A pointer to the UBoxComponent representing the climbable box.
	 */
	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetClimbableBox() const;

	/**
	 * @brief Updates the object every frame.
	 *
	 * @param DeltaTime The time elapsed since the last frame in seconds.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * This is the documentation for the OnBeginOverlapDelegate method.
	 *
	 * @param OverlappedComponent The primitive component that has been overlapped.
	 * @param OtherActor The actor that has overlapped the component.
	 * @param OtherComp The primitive component of the overlapping actor.
	 * @param OtherBodyIndex The index of the body that has overlapped the component.
	 * @param bFromSweep Indicates if the overlap was caused by a sweep.
	 * @param SweepResult The result of the sweep that caused the overlap.
	 */
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlapDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

private:
	/**
	 * \class ClimbableBox
	 *
	 * \brief This class represents a climbable box.
	 *
	 * The ClimbableBox class is used to define a climbable box in a game environment.
	 * It is typically used in conjunction with a UBoxComponent to define the collision properties
	 * of the climbable box.
	 *
	 * \note This class is usually used as a member variable in another class.
	 */
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* ClimbableBox;

protected:
	/**
	 * @brief Callback function for when an overlap ends.
	 *
	 * This function is called when an overlap ends between a primitive component
	 * and another actor's primitive component. It logs a message indicating that
	 * the other actor is no longer overlapping the overlapped component, and updates
	 * the overlapping climbable state of the overlapping actor, if applicable.
	 *
	 * @param OverlappedComponent The primitive component that was overlapped.
	 * @param OtherActor The actor whose primitive component overlapped the overlapped component.
	 * @param OtherComp The primitive component of the other actor that overlapped the overlapped component.
	 * @param OtherBodyIndex The index of the body of the other actor's primitive component that overlapped the
	 * overlapped component.
	 *
	 */
	UFUNCTION()
	void OnEndOverlapDelegate(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	/**
	 * The BeginPlay method is a virtual method called when the actor is spawned into the world or when the
	 * game starts. It is called after the construction of the actor and before the Tick method is called.
	 * This method is typically used for initializing game state, such as setting up collision profiles
	 * and event listeners.
	 *
	 * @param None
	 * @return None
	 */
	virtual void BeginPlay() override;
};
