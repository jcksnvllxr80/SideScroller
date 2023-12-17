// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

/**
 * @brief The ABasePaperCharacter class represents a base paper character in a side-scrolling game.
 *
 * This class inherits from the APaperCharacter class and provides functionality for a character's idle animation,
 * death animation, hurt animation, projectile spawn location, shoot delay time, damage, and handling damage and
 * death actions.
 */
class ABasePaperCharacter;

/**
 * ABaseProjectile is a base class for projectiles in a side-scrolling game.
 * It inherits from AActor and provides functionality for movement, collision,
 * animation, sound, and damage.
 */
UCLASS()
class SIDESCROLLER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Constructor for the ABaseProjectile class.
	 * Initializes the projectile's components and sets replication settings.
	 */
	ABaseProjectile();

	/**
	 * @brief Returns the movement speed of the projectile.
	 *
	 * @return The movement speed of the projectile.
	 */
	UFUNCTION(BlueprintCallable)
	float GetMovementSpeed() const;

	/**
	 * Launches the projectile in the specified direction.
	 *
	 * @param XDirection The horizontal direction in which the projectile should be launched.
	 */
	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(const float XDirection);

	/**
	 * DoCollisionAnimAndSound performs necessary actions when the projectile collides with another actor.
	 *
	 * @param MyOwner The owning actor of the projectile.
	 * @param OtherBasePaperActor The other actor that the projectile collides with.
	 *
	 * This method stops the projectile movement, sets its life span, disables collision, and plays a collision
	 * animation. It also plays a hit sound at the location of the projectile and applies damage to the other actor.
	 */
	UFUNCTION(BlueprintCallable)
	void DoCollisionAnimAndSound(const AActor* MyOwner, ABasePaperCharacter* OtherBasePaperActor);

	/**
	 * @brief Get the Projectile Movement Component of the base projectile.
	 *
	 * @return UProjectileMovementComponent* A pointer to the Projectile Movement Component.
	 */
	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovementComp() const;

	/**
	 * @brief Get the projectile flipbook component.
	 *
	 * @return A pointer to the UPaperFlipbookComponent representing the projectile flipbook
	 *
	 * @note The returned pointer may be nullptr if no flipbook component is set for the projectile.
	 */
	UFUNCTION(BlueprintCallable)
	UPaperFlipbookComponent* GetProjectileFlipbook() const;

private:
	/**
	 * @class TravelAnimation
	 *
	 * @brief Represents a travel animation for a character or object.
	 *
	 * This class is used to store and manipulate a travel animation for a character or object. The animation is
	 * expected to be a flipbook, represented by an instance of UPaperFlipbook. The animation can be accessed for
	 * editing or reading, but not modified directly.
	 *
	 * The animation is meant to be used as an effect during travel, such as walking, running, or flying. It is
	 * meant to be related to the movement of the character or object. The animation can be played, stopped, paused,
	 * or resumed as needed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UPaperFlipbook* TravelAnimation;

	/**
	 * @brief A flipbook used for collision animation effects.
	 *
	 * The CollisionAnimation variable holds a reference to a PaperFlipbook asset. This asset is used for
	 * displaying collision animation effects in the game. The flipbook consists of a series of frames that are
	 * played in a sequential order, creating the illusion of movement or transformation.
	 *
	 * The variable is editable anywhere and can be accessed for reading only. It belongs to the "Effects" category.
	 * In addition, it has the meta attribute (AllowPrivateAccess = "true"), which allows private access to the
	 * variable.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UPaperFlipbook* CollisionAnimation;

	/**
	 * @brief HitSound variable.
	 *
	 * This variable represents the sound that will be played when a hit occurs.
	 * It is editable anywhere and can be categorized under the "Effects" category.
	 */
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* HitSound;

	/**
	 * @brief LaunchSound variable is a reference to a sound effect that will be played when launching an object.
	 *
	 * @details This variable is of type USoundBase*, which is a base class for all sound assets in Unreal Engine 4.
	 * It is marked as EditAnywhere so that it can be modified in the editor. It is also categorized under "Effects"
	 * to group it with other sound-related variables.
	 *
	 * @see USoundBase
	 */
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* LaunchSound;

	/**
	 * @brief The ProjectileFlipbook variable represents the PaperFlipbookComponent used for the projectile actor.
	 *
	 * This variable is of type UPaperFlipbookComponent and is marked as VisibleAnywhere, meaning it can be
	 * accessed and modified from any Blueprint or code, and is displayed in the Details panel of the Editor
	 * under the "Actor" category.
	 */
	UPROPERTY(VisibleAnywhere, Category = Actor)
	UPaperFlipbookComponent* ProjectileFlipbook;

	/**
	 * @brief The time it takes for a collision animation to complete.
	 *
	 * The CollisionAnimationTime variable represents the duration (in seconds) it
	 * takes for a collision animation to complete. This variable can be accessed and
	 * modified in the Unreal Engine editor under the EditAnywhere category.
	 *
	 * @see SetCollisionAnimationTime(), GetCollisionAnimationTime()
	 */
	UPROPERTY(EditAnywhere)
	float CollisionAnimationTime = 1.0;

	/**
	 * @brief CollisionTimerHandle
	 *
	 * This variable represents a timer handle used for managing collision detection in the game.
	 *
	 * The CollisionTimerHandle is an instance of the FTimerHandle struct in Unreal Engine.
	 * It is used to store and manage timers related to collision events in the game.
	 *
	 * The FTimerHandle struct provides functionality to start, stop, and check the status of timers.
	 * It allows for the delayed execution of functions or code after a specified amount of time.
	 *
	 * Example usage:
	 *
	 * ```
	 * // Declare a CollisionTimerHandle variable
	 * FTimerHandle CollisionTimerHandle;
	 *
	 * // Start a timer with a delay of 2 seconds and bind to a function
	 * GetWorld()->GetTimerManager().SetTimer(
	 *		CollisionTimerHandle, this, &ACustomCharacter::OnCollisionDetected, 2.f, false
	 * );
	 *
	 * // Stop the timer
	 * GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
	 *
	 * // Check if the timer is active
	 * bool bIsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(CollisionTimerHandle);
	 * ```
	 *
	 * For more information about the FTimerHandle struct and how to use timers in Unreal Engine,
	 * please refer to the official Unreal Engine documentation.
	 *
	 */
	FTimerHandle CollisionTimerHandle;

	/**
	 * @class ProjectileBox
	 * @brief Represents a capsule component used for collision detection in a projectile.
	 *
	 * The ProjectileBox class is a subclass of UCapsuleComponent and is used to handle collision detection
	 * for a projectile. It is marked as visible anywhere and classified under the Collision category.
	 *
	 * @see UCapsuleComponent
	 */
	UPROPERTY(VisibleAnywhere, Category = Collision)
	UCapsuleComponent* ProjectileBox;

	/**
	 * Projectile movement component.
	 *
	 * This component controls the movement behavior of a projectile.
	 * It is responsible for updating the position and velocity of the projectile over time.
	 * The position and velocity are used to determine the trajectory and collision behavior of the projectile.
	 *
	 * @see UProjectileMovementComponent
	 */
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	UProjectileMovementComponent* ProjectileMovementComp = nullptr;

	/**
	 * @brief The class representing the type of damage.
	 *
	 * This class holds the subclass of UDamageType
	 * that is used to determine the type of damage inflicted.
	 *
	 * The subclass of UDamageType is specified through the
	 * DamageType variable which is an editable default property.
	 *
	 * @note This class does not include example code.
	 * @note This documentation is written using Doxygen.
	 *
	 * @see UDamageType
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	/** Damage variable
	 * This variable represents the amount of damage caused by an action, such as an attack.
	 *
	 * @note This variable can be modified in the Unreal Editor.
	 * @note This variable is read-only in blueprints.
	 *
	 * @property Damage
	 * @brief Represents the amount of damage caused.
	 *
	 * @category Damage
	 * @access EditAnywhere (Unreal Editor), BlueprintReadOnly (Blueprints)
	 * @meta (AllowPrivateAccess = "true")
	 *
	 * @type float
	 *
	 * @default 50.f
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 50.f;

	/**
	 * @brief The movement speed of the object.
	 *
	 * This variable represents the movement speed of the object. It is editable anywhere and can be accessed
	 * for reading only. It belongs to the "Move" category and allows private access.
	 *
	 * @note The default value is 50.0.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 50.f;

	/**
	 * @brief The lifespan of the projectile.
	 *
	 * The ProjectileInLifespan variable determines the length of time the projectile will exist before being destroyed.
	 * It is expressed in seconds.
	 *
	 * @details This variable is editable anywhere and is readable only. It belongs to the "Lifespan" category and has
	 * private access.
	 *
	 * @note The default value of this variable is 0.5 seconds.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lifespan", meta = (AllowPrivateAccess = "true"))
	float ProjectileInLifespan = 0.5f;

	/**
	 * Destroys the actor and clears any existing collision timers.
	 *
	 * @param None.
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();

	/**
	 * Handles the event when the projectile hits something.
	 *
	 * @param HitComp The primitive component that was hit by the projectile.
	 * @param OtherActor The actor that was hit by the projectile.
	 * @param OtherComp The primitive component of the actor that was hit by the projectile.
	 * @param NormalImpulse The impulse applied to the actor when hit by the projectile.
	 * @param Hit The hit result containing information about the hit event.
	 */
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 *
	 * This method is called when the game starts or when the projectile is spawned in the game world.
	 * It sets up the necessary components and properties of the projectile for initialization.
	 *
	 * @param None
	 * @return None
	 */
	virtual void BeginPlay() override;
};
