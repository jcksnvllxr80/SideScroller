// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "SideScroller/Projectiles/BaseProjectile.h"
#include "BasePaperCharacter.generated.h"

/**
 *
 */
class APC_PlayerFox;

/** \class ABasePaperCharacter
 *  \brief This class represents a base paper character in a side scroller game.
 *
 *  \details ABasePaperCharacter is a derived class of APaperCharacter. It provides functionality for managing health,
 *  dealing and taking damage, shooting projectiles, and playing animations.
 */
UCLASS()
class SIDESCROLLER_API ABasePaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	/**
	 * @brief The idle animation asset for the character.
	 *
	 * This property represents the idle animation for the character. It is used by the character to play an animation
	 * when it is in an idle state. The animation is stored as a UPaperFlipbook object, which is a type of Unreal
	 * Engine asset that contains a sequence of sprites that can be played as an animation.
	 *
	 * To assign an idle animation to a character, set this property in the Unreal Editor. You can select an existing
	 * UPaperFlipbook asset in your project or create a new one from scratch
	 *.
	 *
	 * The IdleAnimation property is editable anywhere, which means it can be modified from various places in your code
	 * or through the Unreal Editor. It is also blueprint readable and writable, which allows it to be accessed and
	 * modified from Blueprint scripts.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleAnimation;

	/**
	 * @brief The death animation to play when the character dies.
	 *
	 * The DeathAnimation variable is a pointer to a UPaperFlipbook object,
	 * which represents a flipbook animation that will be played when the character dies.
	 * The animation can be edited and accessed from anywhere, including Blueprints,
	 * allowing for easy customization and integration into the game.
	 *
	 * @note This variable can be modified in the Unreal Editor and in Blueprints.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* DeathAnimation;

	/**
	 * Represents the hurt animation for a character.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* HurtAnimation;

	/**
	 * @brief Represents the location where a projectile will spawn.
	 *
	 * This variable is used to store the spawning location of a projectile. It is a 3D vector
	 * that contains the X, Y, and Z coordinates of the spawn location in the game world.
	 *
	 * The value of this variable can be modified in the Blueprint editor. It has both read and write
	 * access, allowing developers to change the spawn location during gameplay or in the editor.
	 *
	 * Usage example:
	 *
	 * FVector SpawnLocation = ProjectileSpawnLoc;
	 * // Use the spawn location for spawning a projectile
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProjectileSpawnLoc;

	/**
	 * Constructor for the ABasePaperCharacter class.
	 * Initializes the character's default properties and sets up replication.
	 */
	ABasePaperCharacter();

	/**
	 * \brief Called when the game starts or when spawned.
	 *
	 * This method is called when the game starts or when the character is spawned into the world. It is a virtual
	 * method that can be overridden by derived classes to provide custom functionality. By default, it sets the
	 * location of the projectile spawn point.
	 *
	 * \note This method should be called using the BeginPlay() function provided by the Unreal Engine.
	 *
	 * \param None.
	 *
	 * \return None.
	 */
	virtual void BeginPlay() override;

	/**
	 * Retrieves the current shoot delay time.
	 *
	 * @return The shoot delay time in seconds.
	 */
	UFUNCTION(BlueprintCallable)
	float GetShootDelayTime() const;

	/**
	 * Tries to give points and then perform death for the given player character.
	 * If the character is not a Player controlled character, it gives points to the causer of the damage
	 * and then performs death.
	 *
	 * @param DamageCauser The player character causing the damage.
	 */
	UFUNCTION(BlueprintCallable)
	void TryGivingPointsThenDoDeath(APC_PlayerFox* DamageCauser);

	/**
	 * @brief Shoots a projectile.
	 *
	 * The Shoot method is responsible for shooting a projectile. It checks whether
	 * the player can shoot or the enemy can shoot, and then prepares to launch
	 * the projectile accordingly.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Shoot();

	/**
	 * @brief Returns the damage of the character.
	 *
	 * @return The damage value of the character.
	 */
	UFUNCTION(BlueprintCallable)
	float GetDamage() const;

	/**
	 * \brief Sets the damage amount for the character.
	 *
	 * This method sets the damage amount for the character. The damage amount determines how much damage the character
	 * will take.
	 *
	 * \param Damage The amount of damage to set for the character.
	 * \return None.
	 */
	UFUNCTION(BlueprintCallable)
	void SetDamage(const float Damage);

	/**
	 * @brief Get the spawn point for projectiles.
	 *
	 * @return A pointer to the scene component representing the spawn point.
	 *
	 * @param None.
	 */
	UFUNCTION(BlueprintCallable)
	USceneComponent* GetProjectileSpawnPoint() const;

	/**
	 * @brief Performs the death action.
	 *
	 * This method is responsible for performing the death action. It is a blueprint callable
	 * function and can be executed on the server and replicated to all clients. The method
	 * is reliable, meaning that it ensures the RPC call is successfully delivered to the server
	 * and clients.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void DoDeath();

	/**
	 * @brief Pushes the character away from the damage causer.
	 *
	 * This method is responsible for moving the character a certain distance away from the damage causer
	 * after taking damage. It calculates the direction and amount of the push based on the relative positions
	 * of the character and the damage causer. If the damage causer is on the right side of the character,
	 * the push will be towards the left, and vice versa.
	 *
	 * @param DamageCauser The actor that caused the damage.
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void PushHurtCharacter(AActor* DamageCauser);

	/**
	 * @brief Plays the hurt sound.
	 *
	 * This method is a BlueprintCallable, NetMulticast, and Reliable function
	 * which means it can be called from Blueprints, executed on all clients
	 * and guaranteed to be executed in the correct order.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayHurtSound();

	/**
	 * Performs a hurt action on the character.
	 *
	 * This method is used to apply damage on the character. It triggers a hurt animation, sets a timer
	 * to finish the hurt animation, and executes the `PushHurtCharacter` method to handle the damage.
	 * It also plays a hurt sound.
	 *
	 * @param DamageCauser The actor that caused the damage.
	 */
	UFUNCTION(BlueprintCallable)
	void DoHurt(AActor* DamageCauser);

	/**
	 * \fn void TakeDamageRPC(float DamageAmount, AActor* DamageCauser)
	 * \brief Method for taking damage from another actor.
	 *
	 * This method is responsible for taking damage from another actor in the game. It is a server-side method that is
	 * called remotely and validated before execution. The damage amount and the damaging actor are passed as parameters
	 * to the method.
	 *
	 * \param DamageAmount The amount of damage to be inflicted on the target actor.
	 * \param DamageCauser The actor causing the damage.
	 *
	 * \details
	 * This method is marked as BlueprintCallable, which means it can be called from Blueprint graphs. It is also
	 * marked as Server, indicating that it can only be executed on the server. It is defined as Reliable, meaning the
	 * call to this method is guaranteed to arrive and be executed by the server. Additionally, it is defined with the
	 * WithValidation meta specifier, requiring a corresponding validation method to be declared for this method.
	 *
	 * This method can be overridden in derived classes, allowing for customization of the damage-taking behavior
	 * based on specific actor types.
	 *
	 * \see TakeDamageValidation
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void TakeDamageRPC(float DamageAmount, AActor* DamageCauser);

	/**
	 * @brief Takes the given damage and handles it.
	 *
	 * This method is responsible for processing and handling damage applied to the character.
	 *
	 * @param DamageAmount The amount of damage to apply to the character.
	 * @param DamageEvent Information about the damage being applied.
	 * @param EventInstigator The controller responsible for the damage event.
	 * @param DamageCauser The actor that caused the damage.
	 *
	 * @return The amount of damage applied to the character.
	 */
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	/**
	 * @brief Prepares for launching a projectile.
	 *
	 * This method is used to prepare for launching a projectile. It takes a boolean parameter
	 * indicating whether the calling object is a player or not. Based on the value of the
	 * parameter, it determines the yaw rotation of the calling object's sprite or actor. It
	 * then calculates the direction for the projectile by checking if the rotation is close
	 * to 180 degrees. If a projectile class is set, it spawns an instance of the projectile
	 * at the designated spawn location and rotation, sets its owner to the calling object,
	 * and launches it in the calculated direction. If no projectile class is set, it logs a warning.
	 *
	 * @param bIsPlayer Indicates whether the calling object is a player. Default value is true.
	 *
	 * @return None.
	 */
	void PrepProjectileLaunch(bool bIsPLayer);

	/**
	 * Checks if the player character can shoot.
	 *
	 * @return True if the player can shoot, false otherwise.
	 *
	 * @param None.
	 */
	bool PlayerCanShoot();

	/**
	 * Check if the enemy can shoot.
	 *
	 * @return true if the enemy can shoot, false otherwise.
	 *
	 * @note This function checks if the enemy character is alive and able to shoot. If the enemy character is dead,
	 * this function logs an error message and returns false. If the enemy character is not of type
	 * AEnemyCollisionPaperCharacter, this function logs a warning message and returns false.
	 */
	bool EnemyCanShoot();

	/**
	 * Sets the health value of the character.
	 *
	 * @param Health The new health value to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetHealth(float Health);

	/**
	 * @brief Adds health to the character.
	 *
	 * This method increases the character's health by the specified value, clamping the result between 0 and the
	 * default health value.
	 *
	 * @param HealthValue The value to add to the character's health.
	 *
	 * @see SetHealth()
	 */
	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthValue);

	/**
	 * @brief A method to get the current health of the character.
	 *
	 * @return The current health of the character as a float value.
	 *
	 * @note This method is a BlueprintPure function in the HUD category.
	 * @see SetHealth()
	 */
	UFUNCTION(BlueprintPure, Category = "HUD")
	float GetHealth() const;

	/**
	 * @brief Get the default health value.
	 *
	 * This method returns the default health value of the character.
	 *
	 * @return The default health value.
	 * @see DefaultHealth
	 */
	UFUNCTION(BlueprintPure, Category = "HUD")
	float GetDefaultHealth() const;

	/**
	 * Destroys the actor.
	 *
	 * If the actor is a PlayFox, it cleans up instead of destroying it to allow players to spectate.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();

	/**
	 * @brief Callback function called when the character finishes being hurt.
	 *
	 * This function sets the character's sprite flipbook to the idle animation.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void HurtFinishedCallback();

	/**
	 * Checks if the character is dead.
	 *
	 * @return true if the character is dead, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	/**
	 * @brief The sound played when the character dies.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;

	/**
	 * @brief The PainSound variable is a UObject pointer that holds a reference to a sound base asset.
	 *        This variable is editable anywhere which means it can be modified in the editor.
	 * @public
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* PainSound;

private:
	/**
	 \brief Health variable
	 
	 This variable represents the current health of an entity. It is a floating point value that can be accessed from
	 anywhere but cannot be modified directly.
	 
	 The Health variable is replicated, which means its value is duplicated across all connected clients in a
	 multiplayer game.
	 
	 \code{.cpp}
	 float Health = 0;
	 \endcode
	 
	 \remark
	 The initial value of Health is set to 0.
	 */
	UPROPERTY(VisibleAnywhere, replicated)
	float Health = 0;

	/**
	 * @brief The spawn point for projectiles.
	 *
	 * The ProjectileSpawnPoint variable is a scene component used to specify the location where projectiles should
	 * be spawned. It is visible in the editor and can be accessed in blueprints for read-only purposes.
	 * This component is categorized under "Components".
	 *
	 * Usage:
	 *  - Assign a USceneComponent object to this variable to define the location where projectiles will be spawned.
	 *  - Access this variable in blueprints to retrieve the spawn point location for read-only purposes.
	 *
	 * This variable is private access only, meaning it can only be accessed within the owning class and its subclasses.
	 *
	 * @note This variable does not include any example code.
	 */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	/**
	 * @brief The ProjectileClass variable represents the class of the projectile that can be spawned.
	 *
	 * This variable is marked as EditAnywhere and BlueprintReadOnly, allowing it to be modified in the editor and
	 * accessed from blueprints. It belongs to the "Projectile Type" category and has the meta tag "AllowPrivateAccess"
	 * set to true.
	 *
	 * @see ABaseProjectile
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseProjectile> ProjectileClass;

	/**
	 * @brief The time it takes for the death animation to complete.
	 *
	 * This variable represents the duration, in seconds, that the death animation of a character or object takes
	 * to complete. It is used to control the speed and timing of the death animation.
	 *
	 * The value of this variable can be modified in the editor as it is declared as an EditAnywhere property.
	 * By default, it is set to 1.0 second.
	 */
	UPROPERTY(EditAnywhere)
	float DeathAnimationTime = 1.0;

	/**
	 * @brief The time it takes for the hurt animation to play.
	 *
	 * This variable represents the duration, in seconds, for the hurt animation to play.
	 * It is used in the gameplay logic to determine how long the character should remain in the hurt state after
	 * being damaged.
	 *
	 * @note The default value is 0.5 seconds.
	 * @note This variable can be edited in the editor.
	 */
	UPROPERTY(EditAnywhere)
	float HurtAnimationTime = 0.5;

	/**
	 * Timer handle for the death timer.
	 */
	FTimerHandle DeathTimerHandle;

	/**
	 * @brief The handle for the hurt timer.
	 *
	 * This variable is used to track a timer that is used to handle hurt events in the application.
	 * It is an instance of the FTimerHandle class.
	 *
	 * The FTimerHandle class is used to manage and control timers in the application, allowing for the
	 * scheduling and cancellation of timed events.
	 */
	FTimerHandle HurtTimerHandle;

	/**
	 * @brief The amount of damage inflicted by an entity or an action.
	 *
	 * Damage represents the numerical value that determines the impact of an attack or any harmful effect in a game
	 * or simulation. The value of Damage can be customized within the game editor or a designated property panel.
	 *
	 * Usage:
	 *  - Assign a value to set the amount of damage a specific entity or action can inflict.
	 *  - Modify the value during gameplay to simulate different levels of attacks or damage.
	 */
	UPROPERTY(EditAnywhere)
	float Damage = 10.0;

	/**
	 * @brief The delay time (in seconds) between each shot as defined in the Unreal Engine Editor.
	 *
	 * @note This variable can be modified in the Unreal Engine Editor.
	 * @note The delay time determines how long the system waits before allowing the player to shoot again.
	 * @note The default value is 2.0 seconds.
	 */
	UPROPERTY(EditAnywhere)
	float ShootDelayTime = 2.0;

	/**
	 * @brief The amount of force applied to the character when hurt.
	 *
	 * This variable represents the amount of force applied to the character
	 * when they are hurt. The force is applied as a push-back effect. The value
	 * is in units per second.
	 *
	 * @details The default value of this variable is 20.0. It can be modified
	 * in real-time through the Unreal Engine editor as it is marked with the
	 * EditAnywhere specifier. The force applied is relative to the character's
	 * mass and will push them away from the source of damage.
	 *
	 * @see UPROPERTY()
	 * @see EditAnywhere
	 */
	UPROPERTY(EditAnywhere)
	float HurtPushAmount = 20.f;

	/**
	 * @brief Indicates whether the object is dead or alive.
	 *
	 * This variable is of boolean type and is used to keep track of the
	 * status of the object. If the value is true, it means the object is
	 * dead. If the value is false, it means the object is alive.
	 */
	bool bIsDead = false;

protected:
	/**
	 * @brief DefaultHealth variable is used to store the default health value for a character.
	 *
	 * This variable is an Unreal Property (UPROPERTY) that can be edited anywhere.
	 * The value of DefaultHealth is set to 100.0 by default.
	 *
	 * @note This variable is typically used in character classes or game mode classes to define the initial health
	 * value for characters.
	 */
	UPROPERTY(EditAnywhere)
	float DefaultHealth = 100.0;

	/**
	 * @brief Calculates the angle between the character's current floor and the character's UpVector.
	 *
	 * @return The floor angle in degrees.
	 */
	UFUNCTION(BlueprintCallable)
	float GetFloorAngle();

	/**
	 * Retrieves the lifetime replicated properties for this actor.
	 *
	 * This method is called during replication to collect the properties that should be replicated for the actor's
	 * lifetime.
	 *
	 * @param OutLifetimeProps The output array that will contain the lifetime replicated properties.
	 * @see FLifetimeProperty
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
