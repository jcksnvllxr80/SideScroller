#pragma once
#include "CoreMinimal.h"
#include "SideScroller/Climbables/BaseClimbable.h"
#include "SideScroller/Characters/BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"
#include "PC_PlayerFox.generated.h"


/**
 * ASideScrollerGameModeBase
 *
 * The base class for the side scroller game mode.
 */
class ASideScrollerGameModeBase;
/**
 *
 */
class USideScrollerGameInstance;

/**
 * @class APC_PlayerFox
 * @brief Represents a player character in the game.
 *
 * This class extends ABasePaperCharacter and implements the IProjectileInterface.
 * It contains functions to control the player character's movements, interactions,
 * and various gameplay attributes such as accumulated points, number of lives, cherry count, and money count.
 */
UCLASS()
class SIDESCROLLER_API APC_PlayerFox : public ABasePaperCharacter, public IProjectileInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Get the interactable object.
	 *
	 * This method returns the interactable object as a UPrimitiveComponent.
	 *
	 * @return The interactable object as a UPrimitiveComponent.
	 */
	UFUNCTION(BlueprintCallable)
	UPrimitiveComponent* GetInteractableObject() const;

	/**
	 * Set the interactable object for the player.
	 *
	 * @param InteractableObj The UPrimitiveComponent object to set as the interactable object.
	 */
	UFUNCTION(BlueprintCallable)
	void SetInteractableObject(UPrimitiveComponent* InteractableObj);

	/**
	 * @brief Get the accumulated points.
	 *
	 * This method returns the accumulated points for the player.
	 *
	 * @return The accumulated points as an integer value.
	 */
	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetAccumulatedPoints() const;

	/**
	 * Sets the accumulated points for the player.
	 *
	 * @param Points The new accumulated points value.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetAccumulatedPoints(const int Points);

	/**
	 * @brief Retrieves the number of lives remaining.
	 *
	 * This method returns an integer value representing the current number of lives the player has remaining.
	 *
	 * @return The number of lives remaining.
	 *
	 * @note This method does not modify any member variables.
	 */
	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetNumberOfLives() const;

	/**
	 * \brief Sets the number of lives for the player character.
	 *
	 * This method is used to set the number of lives for the player character.
	 * The number of lives determines how many attempts the player will have before losing the game.
	 *
	 * \param NumberOfLives The number of lives to set for the player character.
	 *
	 * \returns None.
	 *
	 * \see GetNumberOfLives()
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetNumberOfLives(const int NumberOfLives);

	/**
	 * @brief Get the current cherry count.
	 *
	 * @return The current cherry count as an integer value.
	 */
	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetCherryCount() const;

	/**
	 * Sets the number of cherries in the player's stash.
	 *
	 * This method is a Blueprint callable function that allows setting the number
	 * of cherries in the player's stash. The number of cherries is passed as the
	 * parameter CherryStash.
	 *
	 * @param CherryStash The number of cherries to set in the player's stash.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetCherryStash(int CherryStash);

	/**
	 * @brief Returns the count of money in the player's stash.
	 *
	 * @return The count of money in the player's stash.
	 */
	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetMoneyCount() const;

	/**
	 * @brief Set the amount of money in the player's money stash.
	 *
	 * This function is responsible for updating the value of the player's money stash with the specified amount.
	 *
	 * @param MoneyAmount The amount of money to set in the player's money stash.
	 * @remarks This function can be called from Blueprint scripts in the HUD category.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetMoneyStash(const int MoneyAmount);

	/**
	 * @brief GetSpectatorsAsStr - Gets the string representation of the spectators.
	 *
	 * @param N/A
	 *
	 * @return FText - The text representation of the spectators.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	FText GetSpectatorsAsStr() const;

	/**
	 * Returns the player name as FText.
	 *
	 * @return The player name as FText.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	FText GetPlayerName() const;

	/**
	 * Sets the SpectatorsStr property of the APC_PlayerFox class.
	 *
	 * This method iterates through the list of spectators and builds a string representation
	 * of their names. The string is then assigned to the SpectatorsStr property.
	 *
	 * @param None.
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetSpectatorsStr();

	/**
	 * @brief Get the array of spectators.
	 *
	 * @return An array of APC_PlayerFox pointers representing the spectators.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectators")
	TArray<APC_PlayerFox*> GetSpectators() const;

	/**
	 * \brief Add a player to the list of spectators.
	 *
	 * \param Spectator The player to add as a spectator.
	 *
	 * This method adds the specified player to the list of spectators for the current player.
	 * The player*/
	UFUNCTION(BlueprintCallable, Category = "Spectators")
	void AddToSpectators(APC_PlayerFox* Spectator);

	/**
	 * Remove a player from the spectators list.
	 *
	 * This method removes the specified player from the spectators list. The spectators list is a list of
	 * players who are currently spectating the game.
	 *
	 * @param Spectator - The player to be removed from the spectators list.
	 *
	 * @see SetSpectatorsStr()
	 */
	UFUNCTION(BlueprintCallable, Category = "Spectators")
	void RemoveFromSpectators(APC_PlayerFox* Spectator);

	/**
	 * @brief Function called when the player dies.
	 *
	 * This function is called to handle player death. It is a reliable server function
	 * that can be called by the client. It is used to check whether the death is valid
	 * and to perform necessary actions accordingly. This function is categorized under
	 * "Death" category.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Death")
	void PlayerDeath();

	/**
	 * @brief Handles the event when the player falls off the level.
	 *
	 * This method is responsible for handling the event when the player character falls off the level.
	 * It calls the PlayerDeath method to handle the death of the player character.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable, Category = "Fall")
	void HandleFallOffLevel();

	/**
	 * @brief Clears the current interactable object.
	 *
	 * This method sets the current interactable object to nullptr, indicating that there is no active interactable
	 * object.
	 *
	 * @param None.
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearInteractableObject();

	/**
	 * @brief Multicast RPC to handle level completion
	 *
	 * This function is a NetMulticast RPC*/
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void DoLevelCompleteMulticastRPC();

	/**
	 * @brief Executes a server RPC to indicate that a level has been completed.
	 *
	 * This function is blueprint callable, server authoritative, and reliable.
	 *
	 * @param None.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void DoLevelCompleteServerRPC();

	/**
	 * @brief The RunAnimation variable is used to store a reference to a PaperFlipbook asset.
	 *
	 * This variable is exposed in the Unreal Engine editor and can be modified in Blueprint scripts.
	 * It represents the animation that should be played when the character is running.
	 *
	 * @note This variable is declared with the UPROPERTY macro to allow for easy integration with Unreal Engine's
	 * reflection system, which enables features like serialization, replication, and Blueprint scripting.
	 *
	 * @see UPaperFlipbook
	 *
	 * @since [insert version number here]
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* RunAnimation;

	/**
	 * @brief Variable representing a jump animation flipbook.
	 *
	 * A jump animation flipbook is a type of UPaperFlipbook object that displays the
	 * frames of an animation sequence to depict a character or object jumping. This
	 * variable is intended to be used as a visual representation of a jump action in a
	 * game or interactive application.
	 *
	 * The 'EditAnywhere' and 'BlueprintReadWrite' UPROPERTY macros indicate that this
	 * variable can be modified in the Unreal Engine editor and can be accessed and
	 * modified from Blueprints. This allows for easy customization and tweaking of
	 * the jump animation behavior.
	 *
	 * Usage Example:
	 *    - In C++: JumpAnimation->Play();
	 *    - In Blueprints: Set Jump Animation node.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* JumpAnimation;

	/**
	 * @brief The FallAnimation variable represents the flipbook animation for a falling object.
	 *
	 * This variable is editable anywhere and can be read and written in blueprints.
	 *
	 * @see UPaperFlipbook
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* FallAnimation;

	/**
	 * \brief A pointer to the PaperFlipbook used for the crouch animation.
	 *
	 * This variable is an EditAnywhere BlueprintReadWrite property, which means it can be modified in the Unreal
	 * Engine editor and accessed in Blueprint scripts. The PaperFlipbook class represents a flipbook, which is a
	 * collection of 2D images or frames that can be played in sequence to create an animation.
	 *
	 * \see UPROPERTY
	 * \see EditAnywhere
	 * \see BlueprintReadWrite
	 * \see UPaperFlipbook
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* CrouchAnimation;

	/**
	 * @brief The ClimbAnimation variable.
	 *
	 * This variable is used to store a reference to a UPaperFlipbook object
	 * that represents the animation for the climb action. It is accessible for editing
	 * in the Unreal Engine editor and can be read from and written to in Blueprints.
	 *
	 * @see UPaperFlipbook
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* ClimbAnimation;

	/** Variable to store the Stop On Ladder animation.
	 *
	 * This variable is used to reference the UPaperFlipbook object that represents the animation played when the
	 * character stops on a ladder. The animation can be edited in the Blueprint editor and can be accessed and
	 * modified in both Blueprint scripts and C++ code.
	 *
	 * Example usage:
	 * \code{cpp}
	 *     // Assign a Stop On Ladder animation to the variable
	 *     StopOnLadderAnimation = LoadObject<UPaperFlipbook>(nullptr, TEXT("Path/To/Flipbook"));
	 *
	 *     // Play the Stop On Ladder animation
	 *     if (StopOnLadderAnimation != nullptr)
	 *     {
	 *         // Play the animation
	 *         PlayAnimation(StopOnLadderAnimation);
	 *     }
	 * \endcode
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* StopOnLadderAnimation;

	/**
	 * @brief The SlideAngleDeg variable determines the angle, in degrees, at which an object can start sliding.
	 *
	 * The SlideAngleDeg variable is a floating-point value that is used to determine the angle, in degrees,
	 * at which an object can start sliding. It is an editable property that can be accessed and modified
	 * in Blueprint scripts and editor interfaces using the EditAnywhere and BlueprintReadWrite attributes.
	 *
	 * The default value of the SlideAngleDeg variable is 15.0.
	 *
	 * Example usage:
	 * @code
	 *     // Set the SlideAngleDeg variable to 30 degrees
	 *     SlideAngleDeg = 30.0f;
	 * @endcode
	 *
	 * @see UPROPERTY()
	 * @see EditAnywhere
	 * @see BlueprintReadWrite
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideAngleDeg = 15.f;

	/**
	 * @brief A variable for displaying a name banner using text rendering component.
	 *
	 * This variable is used to display a name banner in*/
	UPROPERTY(EditAnywhere, Category = Camera)
	UTextRenderComponent* NameBanner;

	/**
	 * @brief The ShootTimerHandle variable is used to keep track of a timer for shooting functionality.
	 *
	 * It is of type FTimerHandle which is a struct provided by Unreal Engine.
	 * This variable is typically used in conjunction with timers to schedule and manage shooting events.
	 *
	 * @see FTimerHandle
	 */
	FTimerHandle ShootTimerHandle;

	/**
	 * @brief The offset used to calculate the respawn location of a character.
	 *
	 * This variable represents the offset from the original spawn location that is used to calculate the respawn
	 * location of a character. It is a vector consisting of three components: X, Y, and Z, representing the
	 * displacement along the x, y, and z axes respectively.
	 *
	 * The variable is decorated with the UPROPERTY macro, which allows it to be edited in the editor. This means
	 * that the value can be modified in real-time while the game is running. The EditAnywhere specifier makes the
	 * variable editable from any location in the editor, including the Details panel.
	 *
	 * It is initialized with the FVector constructor, which takes three arguments representing the initial values
	 * of X, Y, and Z respectively. In this case, the initial X value is 0.0, the initial Y value is 100.0, and
	 * the initial Z value is 0.0. This means that the respawn location will be offset 100 units in the positive
	 * Y direction from the original spawn location.
	 */
	UPROPERTY(EditAnywhere)
	FVector RespawnLocationOffset = FVector(0.0, 0.0, -3000.0);;

	/**
	 * \brief Takes money from the player's stash
	 *
	 * This function takes a monetary value and adds it to the player's money stash.
	 * It also logs a message indicating the new total in the money stash.
	 *
	 * \param MonetaryValue The amount of money to be added to the money stash.
	 *
	 * \return None.
	 *
	 * \note This function is callable from Blueprints.
	 */
	UFUNCTION(BlueprintCallable)
	void TakeMoney(int MonetaryValue);

	/**
	 * Takes a healing value and adds it to the character's health.
	 *
	 * This method is callable from Blueprints.
	 *
	 * @param HealingValue The amount of healing to be applied.
	 */
	UFUNCTION(BlueprintCallable)
	void TakeHealing(float HealingValue);

	/**
	 * Increases the cherry stash of the player fox by the given number of cherries.
	 *
	 * @param NumCherries The number of cherries to be added to the cherry stash.
	 */
	UFUNCTION(BlueprintCallable)
	void TakeCherries(int NumCherries);

	/**
	 * @brief Executes every frame on the Player Controller.
	 *
	 * This method is called every frame and is responsible for updating the Player Controller. It calls the parent
	 * class's Tick method and then proceeds to perform custom logic specific to the Player Controller. Uncommenting
	 * the commented code will enable logging of speed, rotation, and location every 0.5 seconds.
	 *
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Constructs an instance of APC_PlayerFox.
	 *
	 * This method initializes the APC_PlayerFox object and sets various properties such as tick enablement,
	 * camera arm, follow camera, name banner, character movement settings, replication settings, and initial
	 * rotation values.
	 *
	 * @param none
	 * @return none
	 */
	APC_PlayerFox();

	/**
	 * Spectates the next player.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void SpectateNextPlayer();

	/**
	 * Spectates the player.
	 *
	 * This method changes the view target of the player controller to the actor being spectated.
	 * If the player controller is not a subclass of APlayerController, the method returns without
	 * taking any action. If the player being spectated is invalid or its controller cannot be found,
	 * the method returns without taking any action. If setting the view target fails, an error message
	 * is logged.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable)
	void Spectate() const;

	/**
	 
	 * @brief Spectates the previous player in the game.
	 
	 *
	 * This method allows the player to spectate the previous player in the game.
	 * It checks if the player is dead and fetches the last spectated player.
	 * Then it gets the array of players from the game mode and prints the players' list.
	 * It iterates through the array to find the previous player to spectate.
	 * If a player is found, it breaks the loop and sets the player as the new spectated player.
	 * If the last spectated player is still the current player being spectated, it begins spectating the game mode.
	 
	 *
	 
	 * @param None.
	 
	 * @return None.
	 
	 */
	UFUNCTION(BlueprintCallable)
	void SpectatePrevPlayer();

	/**
	 * @brief Use the action.
	 *
	 * This method is used to perform an action. It can only be called on the server and is reliably executed.
	 * It also carries out validation before the execution.
	 *
	 * @param None.
	 *
	 * @return None.
	 *
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void UseAction();

	/**
	 * @brief Set up the player's input component.
	 *
	 * This method is called to initialize the input bindings for the player character.
	 *
	 * @param PlayerInputComponent Pointer to the player's input component.
	 * @see UInputComponent
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/**
	 * Loads the player name from the player profile and sets it to the PlayerName variable.
	 * If the game instance is null, it sets the default PlayerName as the name of the current instance.
	 * If the world is null, it logs a warning and returns without setting the player name.
	 * If the player is the first player controller and the player name is not already set,
	 * it sets the player name from the game instance's player profile if the player's role is Authority or
	 * AutonomousProxy. If the player name is not sent to the server yet and the player's role is AutonomousProxy,
	 * it calls the SendPlayerNameToServer method to send the player name to the server and sets the
	 * bPlayerNameSentToServer flag to true.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void LoadProfilePlayerName();

	/**
	 * @brief Function called when the character begins playing.
	 *
	 * This function is called when the character is spawned or when the game starts. It initializes various components
	 * and sets up the character's initial state.
	 *
	 * @param None
	 *
	 * @return None
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Makes the player character jump.
	 *
	 * This method is called to make the player character perform a jump action.
	 * It overrides the base class Jump() method and adds some additional functionalities.
	 * If the player character is currently in a hurt animation, the jump action is not performed.
	 * The jump action is also not performed if the player character is currently falling or on a ladder.
	 * When the jump action is performed, a jump sound is played and the base class Jump() method is called.
	 */
	virtual void Jump() override;

	/**
	 * \brief Perform a crouch climb down action.
	 *
	 * This method is called to initiate a crouch climb down action. It checks for various conditions
	 * to determine if the character should start sliding down a hill or initiate a crouch climb action.
	 * If the character is not already sliding and its velocity exceeds the sliding threshold, it starts sliding.
	 * If the character is on a hill and its velocity is in the appropriate direction, it starts sliding.
	 * If the character is already sliding and its velocity drops below the sliding threshold, it stops sliding.
	 * If the character is overlapping a climbable object and not sliding, it initiates a climb downward.
	 * Otherwise, it sets the character's state to crouching and adjusts the projectile spawn point location.
	 *
	 * \param None
	 *
	 * \return None
	 */
	UFUNCTION(BlueprintCallable)
	void CrouchClimbDown();

	/**
	 * \brief Stop the crouch climb action.
	 *
	 * When called, this method will stop the current crouch climb action performed by the player character.
	 * If the character is currently crouching, it will stand up and reset relevant properties.
	 * Additionally, if the ShootUpward property is true, it will be set to false.
	 * Finally, the climbing action will be stopped.
	 *
	 * \param None.
	 * \return None.
	 */
	UFUNCTION(BlueprintCallable)
	void StopCrouchClimb();

	/**
	 * ClimbUp method.
	 *
	 * This method is used to climb up an object or ladder. If the character is overlapping a climbable object,
	 * the character's "bOnLadder" variable will be set to true and the character will start climbing at the specified
	 * climb speed. If the character is not overlapping a climbable object, the character's "bIsCrouching" variable
	 * will be set to false, the character will shoot upward, and the projectile spawn point will be set to the upward
	 * spawn location.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void ClimbUp();

	/**
	 * Stops the climbing movement of the player character.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void StopClimb();

	/**
	 * @brief Sets whether the player is overlapping with a climbable object and updates the relevant variables.
	 *
	 * @param bOverlappingClimbable The boolean value indicating whether the player is overlapping with a climbable
	 *							object.
	 * @param OverlappedClimbable A pointer to the climbable object that the player is overlapping with.
	 *                           Assumed to be a valid pointer.
	 *
	 * This function is callable from Blueprints and is primarily designed for internal use.
	 * It is called when the player character overlaps with a climbable object.
	 * The function updates the local state variables based on the value of bOverlappingClimbable
	 * and the properties of the climbable object. It also modifies the movement mode of the player character
	 * if the player is no longer overlapping with a climbable object.
	 */
	UFUNCTION(BlueprintCallable)
	void SetOverlappingClimbable(bool bOverlappingClimbable, ABaseClimbable* OverlappedClimbable);

	/**
	 * Set the transform of a projectile based on the given parameters.
	 *
	 * @param Direction The direction in which the projectile should move.
	 * @param MyOwner The owner of the projectile.
	 * @param BaseChar The base character from which the projectile is fired.
	 * @param Projectile The projectile to set the transform for.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetProjectileTransform(
		const float Direction,
		AActor* MyOwner,
		const ABasePaperCharacter* BaseChar,
		const ABaseProjectile* Projectile
	) override;

	/**
	 * Sets the last checkpoint location for the player.
	 *
	 * @param Location The location of the last checkpoint
	 */
	UFUNCTION(BlueprintCallable)
	void SetLastCheckpointLocation(const FVector& Location);

	/**
	 * Begins spectating the next player in the given game mode.
	 *
	 * @param GameMode - A pointer to the SideScrollerGameModeBase instance.
	 * @param SearchInReverse - If true, the method will search for players to spectate in reverse order. Defaults to
	 * false.
	 */
	UFUNCTION(BlueprintCallable)
	void BeginSpectating(const ASideScrollerGameModeBase* GameMode, bool SearchInReverse);

	/**
	 * Spectates another player.
	 *
	 * Usage:
	 * - Call this method to spectate another player in the game.
	 * - This method can be called from blueprints using the SpectateOtherPlayer function.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void SpectateOtherPlayer();

	/**
	 * Revives the player character at the last checkpoint.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void ReviveAtCheckpoint();

	/**
	 * @brief Performs clean-up actions upon the player's death.
	 *
	 * This method is called when the player dies and is responsible for performing
	 * various clean-up actions such as removing the player from the players array,
	 * tearing down the player's HUD, tearing down the player message widget, and
	 * moving any spectators to a new player.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void DeathCleanUp();

	/**
	 * @brief Print the list of players.
	 *
	 * This function prints the list of players provided in the PlayersArray parameter.
	 * It logs the name of each player along with their status (alive or dead).
	 *
	 * @param PlayersArray The array of player objects.
	 */
	UFUNCTION(BlueprintCallable)
	void PrintPlayersList(TArray<APC_PlayerFox*> PlayersArray);

	/**
	 * Displays a game message on the screen.
	 *
	 * This function is responsible for displaying a game message on the screen. It takes a FText parameter
	 * Message, which is the message to be displayed. The message is then set as the text of the UTextBlock
	 * TextBlock widget, which is retrieved using the GetMessageWidgetTextBlock function.
	 *
	 * If the TextBlock widget is found, the message text is set as its text, and a log message is printed
	 * to the UE_LOG with the level Display. The visibility of the TextBlock widget is also set to Visible.
	 *
	 * @param Message The message to be displayed.
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void DisplayGameMessage(FText Message);

	/**
	 * @brief Hides the game message.
	 *
	 * This method hides the game message by setting the visibility of the associated text block to hidden.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void HideGameMessage() const;

private:
	/**
	 * @brief Indicates whether the player's name has been sent to the server.
	 *
	 * This boolean variable is used to track whether the player's name has been sent
	 * to the server. It is initially set to false and gets updated to true once the
	 * player's name has been successfully sent to the server.
	 *
	 * @see UPROPERTY()
	 *
	 * @remarks Use the UPROPERTY() macro to define this variable as a Unreal Engine 4 property.
	 *
	 * @note This variable does not include any example code.
	 *
	 */
	UPROPERTY()
	bool bPlayerNameSentToServer = false;

	/*****************************************************************************
	 * bPlayerNameSet
	 *****************************************************************************
	 * Description:
	 *    This variable represents whether the player name has been set or not.
	 *    It is a boolean flag that is set to false by default.
	 *
	 * Properties:
	 *    - UPROPERTY
	 *
	 *****************************************************************************/
	UPROPERTY()
	bool bPlayerNameSet = false;

	/**
	 * @brief Sends the player name to the server.
	 *
	 * This function is called to send the player name to the server. It is decorated
	 * with Unreal Engine's UFUNCTION macro, which allows it to be called from Blueprint
	 * and to be executed on the server. It is also marked as BlueprintCallable, which
	 * means it can be called from Blueprint graphs. It is reliable, meaning it will
	 * ensure that the message is delivered and received, and WithValidation, meaning
	 * it will first execute a validation function before calling the server function.
	 *
	 * @param ClientPlayerName The player name to be sent to the server.
	 *
	 * @remark This function should be called when the player wants to send their name
	 *          to the server. The player name should be provided as an argument in the
	 *          form of a FString object. This function will ensure that the player name
	 *          is received by the server reliably and with validation.
	 *
	 * @see Server_SendPlayerNameToServer(const FString& ClientPlayerName)
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SendPlayerNameToServer(const FString& ClientPlayerName);

	/**
	 * @brief Sends the rotation value to the server.
	 *
	 * This method is called to send the rotation value to the server.
	 *
	 * @param Value The rotation value to be sent.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SendRotationToServer(const float Value);

	/**
	 * SendVerticalAxisInputToServer method sends the vertical axis input to the server.
	 *
	 * @param Z The vertical axis input value.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SendVerticalAxisInputToServer(const float Z);

	/**
	 * @brief Boolean variable to indicate whether the object can shoot or not.
	 *
	 * This variable is used to determine if an object is able to shoot or not.
	 * If the value is true, the object is able to shoot. If the value is false,
	 * the object cannot shoot.
	 *
	 * @note By default, the initial value is set to true.
	 */
	UPROPERTY()
	bool bCanShoot = true;

	/**
	 * @brief FRotator variable to store the last rotation.
	 *
	 * This variable is used to store the last rotation value of an object.
	 * It can be used to compare with the current rotation value and perform
	 * certain operations if needed.
	 *
	 * Properties:
	 *  - UPROPERTY(): Specifies that this variable will be accessible from Blueprint
	 *    and is visible in the Unreal Editor.
	 *
	 * @see FRotator
	 */
	UPROPERTY()
	FRotator LastRotation;

	/**
	 * @brief InteractableObject is a variable of type UPrimitiveComponent*.
	 *
	 * This variable represents an object that can be interacted with in the game. It is a pointer to an instance
	 * of the UPrimitiveComponent class, which is a base class for various components that can be used in the game,
	 * such as meshes, particles, and audio components.
	 *
	 * This variable is marked as a UPROPERTY, which means it can be modified and accessed within the Unreal Engine.
	 * It can be used to interact with the object, perform actions, or gather information about its state.
	 *
	 * Example usage:
	 *     // Assign an interactable object to the variable
	 *     InteractableObject = FindComponentByClass<UPrimitiveComponent>();
	 *
	 *     // Check if the object is valid and interact with it if it is
	 *     if (InteractableObject)
	 *     {
	 *         InteractableObject->DoAction();
	 *     }
	 *
	 * @see UPrimitiveComponent
	 */
	UPROPERTY()
	UPrimitiveComponent* InteractableObject;

	/**
	 * @class GameInstance
	 * @brief A variable representing an instance of the SideScrollerGameInstance class.
	 *
	 * The GameInstance variable is a UPROPERTY that holds a pointer to an instance of the USideScrollerGameInstance
	 * class. It is used to access and manage game-specific data and functionality throughout the lifetime of the game.
	 *
	 * This variable provides a central point for storing and accessing game state and information that needs to persist
	 * between levels, such as player scores, current gameplay settings, or saved game data.
	 *
	 * It is recommended to use this variable for accessing game-specific functionality and data in various classes and
	 * components.
	 *
	 * @see USideScrollerGameInstance
	 */
	UPROPERTY()
	USideScrollerGameInstance* GameInstance;

	/**
	 * @brief The WidgetPlayerHUD variable holds a reference to a subclass of the UUserWidget class.
	 *
	 * This variable is editable in the Unreal Engine editor and can be assigned with any subclass of UUserWidget.
	 * It is used to keep track of the player's HUD widget in the game.
	 *
	 * Example usage:
	 *
	 * WidgetPlayerHUD = MyPlayerHUDWidgetClass;
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetPlayerHUD;

	/**
	 * @brief Number of frames per step.
	 *
	 * This variable represents the number of frames that are processed in each step of an animation
	 * or game loop. It determines the smoothness of the animation or the speed of the game.
	 *
	 * @details The value of FramesPerStep should be greater than zero. A higher value will result
	 * in faster animation or game speed, while a lower value will result in slower speed but
	 * smoother animation.
	 *
	 * @note This variable can be modified in the editor as it is annotated with UPROPERTY(EditAnywhere).
	 */
	UPROPERTY(EditAnywhere)
	int32 FramesPerStep = 12;

	/**
	 * WidgetPlayerHUDInstance is a replicated editable property that represents an instance of a user widget used for
	 * player HUD.
	 */
	UPROPERTY(EditAnywhere, replicated)
	UUserWidget* WidgetPlayerHUDInstance;

	/**
	 *
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetPlayerGameMessage;

	/**
	 * @var WidgetPlayerGameMessageInstance
	 * @brief Points to the instance of a user widget used for displaying game messages.
	 *
	 * This variable is a pointer to the UserWidget class which is used to display game messages to the player. It is
	 * declared as a UPROPERTY(), indicating that it can be accessed and modified by Unreal Engine's property system.
	 *
	 * When a game message needs to be displayed to the player, this variable is used to access the instantiated user
	 * widget. The user widget can then be used to update the message and display it on the screen.
	 *
	 * It is important to note that this variable does not contain any example code. The purpose of this documentation
	 * is to explain the purpose and usage of the variable, not to provide code examples.
	 */
	UPROPERTY()
	UUserWidget* WidgetPlayerGameMessageInstance;

	/**
	 * @brief The location of the last checkpoint in the game.
	 *
	 * This variable is of type FVector and is editable anywhere. It represents the location of the last
	 * checkpoint reached by the player in the game.
	 *
	 * @note The value of this variable should be set and updated accordingly whenever the player reaches
	 * a new checkpoint in the game.
	 */
	UPROPERTY(EditAnywhere)
	FVector LastCheckpointLocation;

	/**
	 * @brief Represents the number of lives a player has.
	 *
	 * The NumberOfLives variable is an integer value that represents the current number
	 * of lives a player has. It is editable anywhere and can be replicated across
	 * multiple instances of the game.
	 */
	UPROPERTY(EditAnywhere, replicated)
	int NumberOfLives = 5;

	/**
	 * @brief Variable to store the accumulated points
	 *
	 * This variable is used to store the total accumulated points.
	 */
	UPROPERTY(EditAnywhere, replicated)
	int AccumulatedPoints = 0;

	/**
	 * @brief The speed at which the character can climb.
	 *
	 * This variable represents the climbing speed of the character. It is used
	 * to determine how quickly the character can climb up surfaces or obstacles.
	 * The value is a floating-point number and can be adjusted in the editor.
	 */
	UPROPERTY(EditAnywhere)
	float ClimbSpeed = 0.17f;

	/**
	 * @brief The speed at which the character can move laterally while climbing.
	 *
	 * This variable represents the speed at which the character can move laterally
	 * while climbing. It is a float value that can be edited in the editor.
	 *
	 * @note The default value is 0.4.
	 */
	UPROPERTY(EditAnywhere)
	float ClimbingLateralSpeed = 0.4;

	/**
	 * @brief Floating point variable representing the amount of friction applied when crouching and sliding.
	 *
	 * This variable is used to determine the amount of friction applied to the player character when performing a
	 * crouch slide. The friction affects the speed at which the player character slows down during the crouch slide
	 * motion.
	 *
	 * The variable is editable from the Unreal Engine editor, allowing designers and developers to easily tweak the
	 * value depending on the desired gameplay experience. The default value is 0.175.
	 *
	 * @note Higher values increase friction, causing the player character to slow down faster during crouch slide.
	 * @note Lower values decrease friction, allowing the player character to slide further before coming to a stop.
	 */
	UPROPERTY(EditAnywhere)
	float CrouchSlideFriction = 0.175;

	/**
	 * @brief Represents the standing friction of an object.
	 *
	 * The standing friction determines the resistance to movement when an object is not in motion.
	 */
	float StandingFriction;

	/**
	 * \brief Boolean variable indicating whether the player is out of lives or not.
	 *
	 * This variable is used to determine if the player has run out of lives or not. When set to true,
	 * it indicates that the player cannot continue the game due to lack of remaining lives.
	 * When set to false, it indicates that the player still has lives left and can continue playing.
	 *
	 * \see PlayerController
	 */
	UPROPERTY(VisibleAnywhere, replicated)
	bool bIsOutOfLives = false;

	/**
	 * @brief The normal walking speed variable.
	 *
	 * This variable represents the typical speed at which a person walks.
	 * It is expressed in meters per second (m/s).
	 */
	float NormalWalkingSpeed;

	/**
	 * @brief Maximum running speed.
	 *
	 * This variable represents the maximum speed at which an object can run. The speed is measured in units per
	 * second (UPS). The value can be modified in the editor.
	 *
	 * @note The default value is 500 units per second.
	 */
	UPROPERTY(EditAnywhere)
	float MaxRunningSpeed = 500.f;

	/**
	 * @brief CrouchSlidingThresholdVelocity is a variable that represents the threshold velocity for crouch sliding.
	 *
	 * This variable is used to determine the minimum velocity required for the character to enter into the crouch
	 * sliding state.
	 *
	 * @details
	 *     - EditAnywhere: This variable can be modified through the Unreal Engine editor.
	 *     - double: This variable is of type double.
	 *     - CrouchSlidingThresholdVelocity: The name of the variable.
	 *     - 200.f: The initial value assigned to the variable, representing a threshold velocity of 200 units.
	 */
	UPROPERTY(EditAnywhere)
	double CrouchSlidingThresholdVelocity = 200.f;

	/**
	 * @brief The CherryStash variable represents the number of cherries in a stash.
	 *
	 * The CherryStash variable is an integer that stores the number of cherries in a stash.
	 * It is defined as a UPROPERTY(EditAnywhere) which means it can be edited in the Unreal Engine editor.
	 * The initial value of CherryStash is 0.
	 */
	UPROPERTY(EditAnywhere)
	int CherryStash = 0;

	/**
	 * @brief MoneyStash variable represents the amount of money in the stash.
	 *
	 * It is an integer variable that holds the current amount of money in the stash.
	 * The MoneyStash variable can be edited anywhere.
	 *
	 * Example usage:
	 *     MoneyStash = 1000;
	 *
	 * @see IncreaseMoney(), DecreaseMoney()
	 */
	UPROPERTY(EditAnywhere)
	int MoneyStash = 0;

	/**
	 * @brief Jump sound.
	 *
	 * JumpSound is a variable of type USoundBase* that is used to store the jump sound
	 * associated with an object. It is an editable property that can be modified through
	 * the Unreal Engine Editor.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	/**
	 * @brief The sound played when the character is walking.
	 *
	 * This variable represents the sound that will be played when the character is walking.
	 * It is an instance of the USoundBase class and can be modified in the editor.
	 * The sound is also replicated to all clients in a networked game environment.
	 *
	 * @see USoundBase
	 *
	 * @note This variable should be set to a valid sound asset in order for the character's walking sound to be played
	 * correctly.
	 */
	UPROPERTY(EditAnywhere, replicated)
	USoundBase* WalkSound;

	/**
	 * @brief The sound played when the level starts.
	 *
	 * This variable represents the sound that will be played when the level starts. It is an editable
	 * property, which means it can be modified in the editor. The sound should be of type USoundBase*,
	 * which is a base class for various sound types in Unreal Engine. The sound can be assigned to this
	 * variable using the editor, allowing different levels to have different start sounds.
	 *
	 * Example usage:
	 * ```
	 * UPROPERTY(EditAnywhere)
	 *     USoundBase* LevelStartSound;
	 * ```
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* LevelStartSound;

	/**
	 * @brief The sound played when the level is completed.
	 *
	 * This variable represents the level complete sound played when the player successfully completes a level.
	 * The sound can be edited in the editor and set to any valid USoundBase asset.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* LevelCompleteSound;

	/**
	 * @brief Whether the character is currently crouching or not.
	 *
	 * This variable represents the state of whether the character is currently crouching or not.
	 * It is a boolean value where true indicates that the character is crouching, and false indicates that the
	 * character is not crouching.
	 *
	 * @note This variable can be edited anywhere and is also replicated across the network.
	 */
	UPROPERTY(EditAnywhere, replicated)
	bool bIsCrouching = false;

	/**
	 * @brief Indicates whether the character is currently climbing.
	 *
	 * This boolean variable is used to determine whether the character is currently*/
	UPROPERTY(EditAnywhere, replicated)
	bool bIsClimbing = false;

	/**
	 * @brief Boolean variable indicating whether the character is currently sliding.
	 *
	 * This variable is used to determine if the character is currently sliding. When set to true, it means the
	 * character is in a sliding state, otherwise it is false.
	 */
	UPROPERTY(EditAnywhere, replicated)
	bool bIsSliding = false;

	/**
	 * \brief A boolean variable representing if the character is currently on a ladder.
	 *
	 * This variable is used to store the current state of the character in relation to ladders.
	 * When this variable is true, it indicates that the character is currently on a ladder.
	 * When this variable is false, it indicates that the character is not on a ladder.
	 *
	 * This variable is editable anywhere and replicated across the network.
	 */
	UPROPERTY(EditAnywhere, replicated)
	bool bOnLadder = false;

	/**
	 * @brief Array of spectators.
	 *
	 * This array stores references to the APawn derived objects that are currently observing the gameplay.
	 * The array is editable from anywhere and replicated across the network.
	 */
	UPROPERTY(EditAnywhere, replicated)
	TArray<APC_PlayerFox*> Spectators;

	/**
	 * @brief A variable representing the player currently being spectated.
	 *
	 * This variable stores a reference to the instance of the `APC_PlayerFox` class
	 * that is currently being spectated. The `PlayerBeingSpectated` variable is
	 * editable anywhere and is replicated across the network.
	 *
	 * By accessing this variable, other parts of the code can obtain information
	 * about the player being spectated and perform actions accordingly. This can be
	 * useful in scenarios where the gameplay or UI needs to react and display
	 * information specific to the spectated player.
	 *
	 * It is important to note that this variable should be updated as the currently
	 * spectated player changes to ensure that other parts of the code always have
	 * access to the correct player instance.
	 *
	 * Usage example:
	 *     APC_PlayerFox* SpectatedPlayer = PlayerBeingSpectated;
	 *     if (SpectatedPlayer)
	 *     {
	 *         // Perform actions specific to the spectated player
	 *     }
	 *     else
	 *     {
	 *         // No player is currently being spectated
	 *     }
	 */
	UPROPERTY(EditAnywhere, replicated)
	APC_PlayerFox* PlayerBeingSpectated;

	/**
	 * Indicates whether the object is currently overlapping with a climbable surface.
	 *
	 * This variable is used to determine if the object can be climbed or interacted with
	 * when it is overlapping with a climbable surface. When set to true, it indicates that
	 * the object is in a climbable state, otherwise it is not climbable.
	 *
	 * @see UPROPERTY
	 */
	UPROPERTY(EditAnywhere)
	bool bOverlappingClimbable;

	/**
	 * @brief The CameraArm variable holds the reference to the USpringArmComponent used for the camera.
	 *
	 * This variable is primarily used for controlling the positioning and movement of the camera.
	 *
	 * @see USpringArmComponent
	 * @see VisibleAnywhere
	 * @see Category
	 */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraArm;

	/**
	 * \brief The FollowCamera variable represents the camera component that*/
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	/**
	 * @brief The sound that plays when the player is near a climbable object.
	 *
	 * This variable holds a reference to a USoundBase object which represents the sound that will play when the player
	 * is near a climbable object. The sound will be played in the game to provide audio feedback to the player about
	 * the presence of a nearby climbable object.
	 *
	 * @see USoundBase
	 * @note This variable is defined as a UPROPERTY() to support Unreal Engine's property system.
	 */
	UPROPERTY()
	USoundBase* NearbyClimbableSound;

	/**
	 * @brief Variable to store the cumulative time.
	 *
	 * This variable is editable from anywhere. It is used to keep track of the cumulative time.
	 * The value of this variable is stored as a float and is initialized to 0.0.
	 */
	UPROPERTY(EditAnywhere)
	float CumulativeTime = 0.f;

	/**
	 * @brief The rotation of the current object.
	 */
	UPROPERTY(EditAnywhere, replicated)
	FRotator CurrentRotation;

	/**
	 * @brief The rotation applied when moving left.
	 *
	 * This property represents the rotation that will be applied when an object is moving left.
	 * The rotation is represented by a FRotator object.
	 *
	 * Usage example:
	 *
	 *     FRotator rotation = MovingLeftRotation;
	 *     // Apply rotation to object
	 *
	 * @see FRotator
	 */
	UPROPERTY(EditAnywhere)
	FRotator MovingLeftRotation = FRotator(0, 180.f, 0);

	/**
	 * @brief The FRotator property that determines the rotation applied when moving right.
	 *
	 * The MovingRightRotation variable is a FRotator property used to specify the rotation to be applied when moving
	 * right. It is an editable property, meaning it can be modified in the editor or through code. By default, it is
	 * set to (0, 0, 0) which represents no rotation.
	 *
	 * @see FRotator
	 *
	 * @note This variable should be used in conjunction with movement logic to apply the desired rotation when moving
	 * right. It can be accessed and modified from both the editor and code.
	 */
	UPROPERTY(EditAnywhere)
	FRotator MovingRightRotation = FRotator(0, 0, 0);

	/**
	 * Moves all spectators to a new player.
	 *
	 * This function is used to move all the spectators from the current player to a new*/
	UFUNCTION(BlueprintCallable)
	void MoveSpectatorsToNewPlayer() const;

	/**
	 * Checks if a player is valid to be spectated and sets them as the player being spectated.
	 * If the player being spectated already has a spectator, it removes the current spectator before setting the new
	 * one.
	 *
	 * @param Player The player to be set as the player being spectated.
	 * @return true if the player is valid and successfully set as the player being spectated, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool FoundPlayerToSpectate(APC_PlayerFox* Player);

	/**
	 * Callback function to handle climb up input on a specific axis.
	 *
	 * This function is called when the climb up input is detected on a specific axis.
	 * It checks the value of the input and calls the appropriate functions to handle the climb up action.
	 * If the input value is greater than 0, it calls the ClimbUp() function.
	 * If the input value is less than 0, it calls the CrouchClimbDown() function.
	 * If the input value is 0, it calls the StopCrouchClimb() function.
	 *
	 * If the current player controller is an autonomous proxy, it sends the vertical axis input value
	 * to the server by calling the SendVerticalAxisInputToServer() function.
	 *
	 * Note: This function is declared as a BlueprintCallable, which means it can be called from Blueprint graphs.
	 *
	 * @param Z The input value on the climb up axis. A positive value indicates climbing up, a negative value indicates
	 * crouch climbing down, and 0 indicates no input.
	 */
	UFUNCTION(BlueprintCallable)
	void ClimbUpAxisInputCallback(float Z);

	/**
	 * Moves the character horizontally to the right based on the input axis.
	 *
	 * @param Axis The input axis value representing the movement direction.
	 *             Positive values move the character to the right, while
	 *             negative values move the character to the left.
	 */
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Axis);

	/**
	 * @brief Climb method used to make the player character climb.
	 *
	 * This method is used to make the player character climb. It takes a float value as input which determines
	 * the climbing speed. If the player character is not currently climbing, it stops the character's movement
	 * immediately and sets the climbing flag to true. The movement mode is then set to flying, and the player
	 * character is moved vertically based on the input value and the actor's up vector.
	 *
	 * @param Value The float value that determines the climbing speed.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void Climb(float Value);

	/**
	 * @brief Logs the speed of the player.
	 *
	 * This function uses Unreal Engine's logging system to display the speed of the player.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void LogSpeed();

	/** LogRotation function logs the rotation of the player character in the output log.
	 
	 @param None
	 @return None
	 
	 @remark This function is a BlueprintCallable function, which means it can be called from Blueprints.
	 
	 @see UFUNCTION
	 @see BlueprintCallable
	 @see LogTemp
	 @see VeryVerbose
	 @see GetName
	 @see GetSprite
	 @see GetRelativeRotation
	 @see ToString
	 */
	UFUNCTION(BlueprintCallable)
	void LogRotation();


	/**
	 * \brief Sets the run velocity via RPC (Remote Procedure Call).
	 *
	 * This method is callable from blueprint and is executed on the server side. It is reliable and includes
	 * validation to ensure that it is executed only on the server.
	 *
	 * \param None.
	 *
	 * \return None.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetRunVelocityRPC();

	/**
	 * @brief Sets the run velocity of the player character.
	 *
	 * This method is used to set the maximum walk speed of the player character to the desired running speed. If the
	 * current role of the player is autonomous proxy, it will also call the SetRunVelocityRPC method to update the
	 * run velocity on the server.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void SetRunVelocity();

	/**
	 * Sets the walk velocity via RPC (Remote Procedure Call) to the server.
	 * This function is called by clients to update the walk velocity on the server.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetWalkVelocityRPC();

	/**
	 * @brief Set the walking velocity of the player character.
	 *
	 * This function sets the maximum walk speed of the player character to a normal walking speed value.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void SetWalkVelocity();

	/**
	 * @brief Logs the location of the actor.
	 *
	 * This function logs the location of the actor using Unreal Engine's logging system. The location is logged in
	 * verbose mode.
	 *
	 * @param None.
	 *
	 * @return None.
	 *
	 * @see UE_LOG
	 */
	UFUNCTION(BlueprintCallable)
	void LogLocation();

	/**
	 * Opens the menu for the player.
	 *
	 * This method is used to open the menu for the player. If the player is in a level game mode, the in-game menu
	 * will be opened. If the player is in a lobby game state, the character selection menu will be opened.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void OpenMenu();

	/**
	 * \brief Opens the in-game menu.
	 *
	 * This method is responsible for opening the in-game menu. It sets the game pause state to true and calls the
	 * InGameLoadMenu() method of the game instance. If the game instance is null, a warning message is logged.
	 *
	 * \param None.
	 * \return None.
	 *
	 * \sa APC_PlayerFox::InGameLoadMenu
	 */
	UFUNCTION(BlueprintCallable)
	void OpenInGameMenu();

	/**
	 * @brief Sets the value of bCanShoot to true, indicating that the player can shoot again.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION()
	void CanShootAgain();

	/**
	 * @brief Shoots a projectile if the player is able to shoot
	 *
	 * This method is called to shoot a projectile if the player is able to
	 * shoot. The bCanShoot flag is checked to determine if the player can
	 * currently shoot. If the flag is true, the Shoot() method in the base
	 * class is called to perform the shooting logic. After shooting, the
	 * bCanShoot flag is set to false to prevent shooting again immediately.
	 * A timer is then set to allow the player to shoot again after a delay.
	 *
	 * @note This method is a virtual method and overrides the Shoot() method
	 *       in the base class.
	 *
	 * @param None
	 *
	 * @return None
	 */
	virtual void Shoot() override;

	/**
	 * @brief Plays the walk animation and sound.
	 *
	 * This method is used to play the walk animation and sound for the player fox character.
	 * It checks if the current animation is not the run animation and sets it to the run animation.
	 * It also checks the playback position of the animation and if it is divisible evenly by a specified number of frames,
	 * it spawns the walk sound attached to the character sprite.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void DoWalkAnimAndSound();

	/**
	 * Function to perform climb animation and play climbing sound based on the player's velocity.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void DoClimbAnimAndSound();

	/**
	 * Add this player controller to the game mode's players array.
	 *
	 * Note that this method assumes that the game mode is a subclass
	 * of ASideScrollerGameModeBase.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void AddToPlayersArray();

	/**
	 * Removes the current player from the Players Array in the game mode.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveFromPlayersArray();

	/**
	 * Sets up the player HUD.
	 *
	 * @param None.
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void PlayerHUDSetup();

	/**
	 * Tears down the Player HUD by removing the WidgetPlayerHUDInstance from the parent.
	 * If the WidgetPlayerHUDInstance is null, a warning message is logged.
	 *
	 * @param None
	 *
	 * @returns None
	 */
	UFUNCTION(BlueprintCallable)
	void PlayerHUDTeardown();

	/**
	 * @brief Teardown the Player Message Widget.
	 *
	 * This function is responsible for tearing down the Player Message Widget instance.
	 * If the WidgetPlayerGameMessageInstance is valid, it will be removed from its parent
	 * and destroyed. If the WidgetPlayerGameMessageInstance is null, a warning message will
	 * be logged.
	 *
	 * @note It is assumed that the Player Message Widget instance has been initialized before
	 *       calling this function.
	 *
	 * @see WidgetPlayerGameMessageInstance
	 */
	UFUNCTION(BlueprintCallable)
	void PlayerMessageWidgetTeardown();

	/**
	 * Sets up the player game message widget.
	 *
	 * This method creates an instance of the player game message widget and adds it to the viewport.
	 * It also calls the HideGameMessage() method to hide the game message initially.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void PlayerGameMessageSetup();

	/**
	 * \brief Retrieves the UTextBlock associated with the game message widget.
	 *
	 * This method is a BlueprintCallable function that allows access to the UTextBlock
	 * widget used for displaying game messages. This method takes a UTextBlock pointer
	 * as an output parameter, which will be set to the UTextBlock widget if it exists.
	 *
	 * \param[in,out] TextBlock A reference to a UTextBlock pointer that will be set to the
	 *                          UTextBlock widget if it exists.
	 *
	 * \remarks If the WidgetPlayerGameMessageInstance member variable is not null, this
	 * method attempts to retrieve the UTextBlock widget using the name "UserMessageTB" from
	 * the WidgetPlayerGameMessageInstance. If successful, the TextBlock parameter will be
	 * set to the UTextBlock widget. If the WidgetPlayerGameMessageInstance is null, an
	 * appropriate warning message will be logged.
	 *
	 * \see UTextBlock
	 * \see WidgetPlayerGameMessageInstance
	 * \see FName
	 *
	 * \par Example Usage:
	 * \code{.cpp}
	 * UTextBlock* MessageWidget;
	 * GetMessageWidgetTextBlock(MessageWidget);
	 * if (MessageWidget != nullptr)
	 * {
	 *     // Use MessageWidget...
	 * }
	 * \endcode
	 */
	UFUNCTION(BlueprintCallable)
	void GetMessageWidgetTextBlock(UTextBlock*& TextBlock) const;

	/**
	 * Brief description of the method DoLevelWelcome.
	 *
	 * This method displays a level welcome message if the game state is a SideScrollerGameState and the current level
	 * name contains "Level". It also plays a level start sound and hides the message after a specified amount of time.
	 * If the game state is not a SideScrollerGameState or the current level name does not contain "Level", a warning
	 * message is logged.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION()
	void DoLevelWelcome();

	/**
	 * @brief The position where the crouched projectile should be spawned.
	 */
	UPROPERTY(EditAnywhere)
	FVector CrouchProjectileSpawnPoint = FVector(0.f, 0.f, 5.f);

	/**
	 * @brief The location where the projectile should be spawned above an actor.
	 *
	 * This variable specifies the upward spawn location for a projectile with respect to an actor.
	 * The projectile will be spawned at this location above the actor's position.
	 *
	 * @details The default value for this variable is (0.0, 0.0, 10) in the world coordinate system.
	 * The values of this variable can be modified in the editor as it is marked with the EditAnywhere attribute.
	 *
	 * @note The value of this variable is a FVector, which represents a 3D vector in the world coordinate system.
	 *
	 * @see FVector
	 */
	UPROPERTY(EditAnywhere)
	FVector ProjectileUpwardSpawnLoc = FVector(0.0, 0.0, 10);

	/**
	 * @brief Boolean variable indicating whether the object should shoot upward.
	 *
	 * This variable determines if the object should shoot upward or not. The default value is false, meaning the
	 * object will not shoot upward. It can be modified using an editor.
	 */
	UPROPERTY(EditAnywhere)
	bool ShootUpward = false;

	/**
	 * @brief The string representing the spectators.
	 *
	 * This variable is of type FString and is annotated with UPROPERTY macro for visibility and replication.
	 * It is visible anywhere and replicated.
	 */
	UPROPERTY(VisibleAnywhere, replicated)
	FString SpectatorsStr;

	/**
	 * @brief The name of the player.
	 *
	 * This variable represents the name of the player. It is of type FString and is editable anywhere in the blueprint
	 * editor. The value of the PlayerName variable is replicated across the network.
	 */
	UPROPERTY(EditAnywhere, Replicated)
	FString PlayerName;

	/**
	 * @brief Timer handle for the level start message.
	 *
	 * This variable is used to store a timer handle for the level start message.
	 * A timer handle is an opaque handle used to identify a running timer. It
	 * is created when starting a timer and can be used to stop or check the
	 * status of the timer later on.
	 *
	 * Use this variable to control the level start message timer.
	 * For example, you can start the timer using the UWorld::GetTimerManager().SetTimer()
	 * function and pass this handle as the first parameter. You can also use the
	 * handle to clear the timer using the UWorld::GetTimerManager().ClearTimer()
	 * function or check if the timer is active using the UWorld::GetTimerManager().IsTimerActive()
	 * function.
	 *
	 * @see UWorld::GetTimerManager()
	 *
	 */
	FTimerHandle LevelStartMessageTimerHandle;

	/**
	 * @brief Handle for the timer used to display the level complete message.
	 *
	 * This variable is used to track and manage the timer that is responsible for displaying
	 * the level complete message. It is of type FTimerHandle, which is a handle to a timer
	 * that can be used to start, pause, resume, or cancel the timer.
	 *
	 * Whenever the level is completed, the timer is started to display the level complete
	 * message for a certain duration. This handle is used to reference the timer and perform
	 * operations on it as required.
	 *
	 * Example usage:
	 *
	 * FTimerHandle LevelCompleteMessageTimerHandle;
	 *
	 * void StartLevelCompleteTimer()
	 * {
	 *     // Start the timer to display the level complete message for 5 seconds
	 *     float DisplayTime = 5.0f;
	 *     GetWorld()->GetTimerManager().SetTimer(LevelCompleteMessageTimerHandle, DisplayTime, false);
	 * }
	 *
	 * void StopLevelCompleteTimer()
	 * {
	 *     // Stop the timer if it is active
	 *     GetWorld()->GetTimerManager().ClearTimer(LevelCompleteMessageTimerHandle);
	 * }
	 *
	 * As shown in the example, you can start the timer by calling `SetTimer` and passing in
	 * the LevelCompleteMessageTimerHandle along with the desired display time. To stop the
	 * timer, you can use `ClearTimer` on the timer manager.
	 */
	FTimerHandle LevelCompleteMessageTimerHandle;

	/**
	 * @brief Timer handle for the level welcome delay.
	 *
	 * This variable represents the timer handle used to manage the delay for the level welcome.
	 * It is an instance of FTimerHandle class.
	 *
	 * The timer handle is used to start, stop, and manage timers in the game.
	 * It is primarily used to delay certain actions or events in the game, such as displaying the level welcome message.
	 *
	 * Example usage:
	 * @code
	 * FTimerHandle LevelWelcomeDelayTimerHandle;
	 *
	 * void StartLevelWelcomeDelay()
	 * {
	 *     // Start a timer with 3 seconds delay for the level welcome message
	 *     GetWorld()->GetTimerManager().SetTimer(LevelWelcomeDelayTimerHandle, this, &AWelcomeHUD::ShowLevelWelcome,
	 *     3.0f, false);
	 * }
	 *
	 * void StopLevelWelcomeDelay()
	 * {
	 *     // Stop the level welcome timer if it is currently running
	 *     if (GetWorld()->GetTimerManager().IsTimerActive(LevelWelcomeDelayTimerHandle))
	 *     {
	 *         GetWorld()->GetTimerManager().ClearTimer(LevelWelcomeDelayTimerHandle);
	 *     }
	 * }
	 * @endcode
	 */
	FTimerHandle LevelWelcomeDelayTimerHandle;

	/**
	 * Represents the amount of time (in seconds) to delay before displaying the level welcome message.
	 *
	 * This variable is a property that can be edited in the editor. Its default value is 0.5 seconds.
	 */
	UPROPERTY(EditAnywhere)
	float LevelWelcomeDelayTime = 0.5;

	/**
	 * @var LevelStartMessageTime
	 * @brief The amount of time (in seconds) to display the level start message.
	 *
	 * This variable represents the duration for which the level start message is shown.
	 * It is used to control how long the message stays visible on the screen before it disappears.
	 *
	 * @note The value is editable anywhere, allowing designers to easily adjust the duration to suit their needs.
	 * The default value is 3.0 seconds.
	 */
	UPROPERTY(EditAnywhere)
	float LevelStartMessageTime = 3.f;

	/**
	 * @brief LevelCompleteMessageTime is a float variable representing the time (in seconds) that the level complete
	 *        message should be displayed on the screen.
	 *
	 * @details This variable is used in the context of game development to determine the length of time for which the
	 *          level complete message should be shown to the player after completing a level. The value is set to 3.0
	 *          by default and can be modified in the editor.
	 *
	 * @note This variable is marked with the UPROPERTY macro for reflection and editor integration purposes.
	 */
	UPROPERTY(EditAnywhere)
	float LevelCompleteMessageTime = 3.f;
	
protected:
	/**
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling;

	/**
	 * @brief Updates the current animation based on the player's state and movement.
	 *
	 * This method is used to update the animation of the player character based on their current state and movement.
	 * It checks for different conditions such as falling, crouching, climbing, and walking in order to set the
	 * appropriate animation flipbook.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimation();

	/**
	 * @brief Updates the rotation of the player character based on the given value.
	 *
	 * This function updates the rotation of the player character based on the given value. If the value is less than 0,
	 * the character's current rotation will be set to MovingLeftRotation and the projectile spawn point will be
	 * mirrored on the X-axis. If the value is greater than 0, the character's current rotation will be set to
	 * MovingRightRotation.
	 *
	 * @param Value The value used to determine the new rotation.
	 * @return None.
	 * @see GetProjectileSpawnPoint(), GetSprite(), SetRelativeRotation(), SetRelativeLocation(), SendRotationToServer()
	 */
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateRotation(const float Value);

	/**
	 * @brief Update the name banner in the animation properties.
	 *
	 * This method updates the name banner in the animation properties. If the current name banner text is empty,
	 * it calls the LoadProfilePlayerName method to load the player name and sets the name banner text to the player
	 * name.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateNameBanner();
};
