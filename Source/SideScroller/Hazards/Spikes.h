// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "Spikes.generated.h"

/**
 * @class ASpikes
 *
 * This class represents a set of spikes in a side-scrolling game. It inherits from APaperSpriteActor
 * and provides functionality for dealing damage to other actors when they collide with the spikes.
 */
UCLASS()
class SIDESCROLLER_API ASpikes : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for ASpikes class.
	 */
	ASpikes();

private:
	/**
	 * @brief Reference to the PaperSpriteComponent for the spikes sprite.
	 *
	 * This variable is editable in the Unreal Editor and is used to specify the PaperSpriteComponent
	 * that will be rendered as the spikes sprite.
	 */
	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* SpikesSprite;

	/**
	 * @brief Represents a box component for spikes.
	 */
	UPROPERTY(EditAnywhere)
	UBoxComponent* SpikesBox;

	/**
	 * @brief The DamageValue variable represents the amount of damage caused by an attack or action.
	 *
	 * @details The DamageValue variable is a float value that specifies the amount of damage caused by an
	 * attack or action. This value is typically used in the game logic to determine the impact or consequence
	 * of the attack/action. The default value of the DamageValue variable is 50.0.
	 */
	UPROPERTY(EditAnywhere)
	float DamageValue = 50.0;

protected:
	/**
	 * Called when the actor is hit by another actor's primitive component.
	 *
	 * @param HitComponent - The primitive component that was hit.
	 * @param OtherActor - The actor that hit this actor.
	 * @param OtherComp - The primitive component that hit this actor.
	 * @param NormalImpulse - The impulse applied to this actor.
	 * @param Hit - Information about the hit.
	 */
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	/**
	 * @brief Called when the game starts or when spawned.
	 *
	 * This method is called when the game starts or when the actor is spawned in the level. It is an overridden method
	 * from the parent class `AActor`.
	 *
	 * @details
	 * In this method, the collision profiles are set for the SpikesSprite and SpikesBox components. The collision
	 * profile "BlockAll" is set for both the components using the `SetCollisionProfileName` function. This ensures
	 * that the actor blocks all collision events.
	 *
	 * Additionally, the flag `bNotifyRigidBodyCollision` of the SpikesBox component is set to true using the
	 * `SetNotifyRigidBodyCollision` function. This is required for the OnComponentHit event to be triggered.
	 *
	 * Finally, the OnComponentHit event is bound to the `ASpikes::OnHit` dynamic delegate function using the
	 * `AddDynamic` function.
	 *
	 * @see SpikesSprite
	 * @see SpikesBox
	 * @see SetCollisionProfileName
	 * @see SetNotifyRigidBodyCollision
	 * @see OnComponentHit
	 * @see ASpikes::OnHit
	 * @see AActor
	 *
	 * @return void
	 */
	UFUNCTION()
	virtual void BeginPlay() override;
};
