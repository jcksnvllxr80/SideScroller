// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
#include "SelectCharacterMenu.generated.h"

/**
 * @class APC_PlayerFox
 *
 * @brief The APC_PlayerFox class represents a player character in a side-scrolling game.
 *
 * The APC_PlayerFox class inherits from the ABasePaperCharacter class and implements the IProjectileInterface.
 *
 * The class provides various methods for interacting with the player character, such as getting and setting the
 * interactable object, managing points, lives, cherries, money, and spectators, handling player death and level
 * completion, and playing animations.
 */
class APC_PlayerFox;
/**
 * USelectCharacterMenu is a subclass of UMenuWidget that represents the character selection menu in the game.
 * This menu allows the player to choose their character from a list of available options.
 */
UCLASS()
class SIDESCROLLER_API USelectCharacterMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief The PinkPlayerBP variable is a reference to the blueprint class "APC_PlayerFox" for the Pink player.
	 *
	 * This variable is an editable property that can be modified in the editor.
	 * It belongs to the "Classes" category and can be read and modified in Blueprints.
	 *
	 * The default value of the PinkPlayerBP is nullptr, indicating that no specific Pink player blueprint is
	 * assigned. In order to assign a specific blueprint, you can change the value of this variable in the editor
	 * or in Blueprints.
	 *
	 * @see APC_PlayerFox
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> PinkPlayerBP = nullptr;

	/**
	 * Variable for storing the blueprint class used to spawn the Orange Player character.
	 *
	 * The OrangePlayerBP variable is of type TSubclassOf<APC_PlayerFox>, which is a template class that allows for
	 * storing a reference to a blueprint class. This blueprint class is used to create instances of the Orange
	 * Player character in the game.
	 *
	 * The variable is decorated with the EditAnywhere and BlueprintReadWrite UPROPERTY macros, which enables the
	 * class variable to be modified in the Unreal Engine editor and accessed from blueprints to spawn the Orange
	 * Player character.
	 *
	 * The Category parameter is set to "Classes", which group the variables in this category in the Unreal Engine
	 * editor for better organization.
	 *
	 * The initial value of the OrangePlayerBP variable is set to nullptr, which means that if no blueprint class
	 * is assigned, the default value will be null.
	 *
	 * @see APC_PlayerFox
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> OrangePlayerBP = nullptr;

	/**
	 * @brief The Blueprint subclass of the Yellow player character.
	 *
	 * This variable represents the Blueprint subclass of the Yellow player character.
	 * It is used to spawn an instance of the Yellow player character in the game world.
	 *
	 * @details This variable is of type TSubclassOf<APC_PlayerFox> and allows for easy selection of the Yellow
	 * player character from within the Unreal Editor Blueprint system. It can be assigned a Blueprint class
	 * that derives from the base class APC_PlayerFox.
	 *
	 * The "EditAnywhere" property specifier allows this variable to be modified in the Unreal Editor.
	 * The "BlueprintReadWrite" property specifier enables it to be accessed and modified from Blueprint scripts.
	 * It is categorized under "Classes" to help organize related variables.
	 *
	 * The default value of this variable is nullptr, meaning it is initially not assigned to any class.
	 * This allows for easy detection of whether a valid Blueprint class has been assigned at runtime.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> YellowPlayerBP = nullptr;

	/**
	 * @brief The GreenPlayerBP variable is a blueprint class pointer property that allows assigning a subclass
	 *        of APC_PlayerFox to it. This variable is editable anywhere and readable/writable in Blueprints.
	 *        It belongs to the "Classes" category.
	 *
	 * @details This variable is used to specify the class of the green player blueprint that will be used in
	 *          the game. It should be assigned to a subclass of APC_PlayerFox. The initial value of the
	 *          GreenPlayerBP variable is nullptr, which means it is not pointing to any subclass at the start.
	 *          It can be modified in Blueprint editor and accessed in Blueprints for gameplay purposes.
	 *
	 * @see APC_PlayerFox
	 *
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> GreenPlayerBP = nullptr;

	/**
	 * @brief The BluePlayerBP variable represents the blueprint class used to spawn the blue player character.
	 *
	 * The BluePlayerBP variable is a TSubclassOf type that allows selecting a blueprint class from the property
	 * editor in Unreal Engine. It is used to determine the blueprint class to spawn when creating instances
	 * of the blue player character.
	 *
	 * @details The blueprint class selected for the BluePlayerBP variable must be a subclass of the
	 * APC_PlayerFox class.
	 *
	 * @note The default value for the BluePlayerBP variable is nullptr, meaning no blueprint class is initially
	 * assigned. It is recommended to assign a valid blueprint class before using this variable to avoid
	 * crashes or unexpected behavior.
	 *
	 * @see APC_PlayerFox
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> BluePlayerBP = nullptr;

	/**
	 * @brief The class variable BlackPlayerBP is a blueprint reference to the Black Player Fox class.
	 *
	 * BlackPlayerBP is used to spawn instances of the Black Player Fox class during gameplay. It is a subclass
	 * of APC_PlayerFox. The variable is editable in Blueprint, allowing designers to specify the Black Player
	 * Fox class they want to use. This variable is visible and can be modified in the Blueprint editor.
	 * The Black Player Fox class must be derived from APC_PlayerFox.
	 * The default value for BlackPlayerBP is nullptr, meaning no Black Player Fox class is selected by default.
	 * Set a valid Black Player Fox class to BlackPlayerBP using the Blueprint editor or through C++ code.
	 *
	 * @see APC_PlayerFox
	 *
	 * @since The variable was first introduced in the project.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> BlackPlayerBP = nullptr;

protected:
	/**
	 * Selects the pink player.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION()
	void PinkPlayerSelect();

	/**
	 * @brief Selects the orange player character.
	 *
	 * This method is used to select the orange player character in the select character menu.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION()
	void OrangePlayerSelect();

	/**
	 * @brief Selects the yellow player character.
	 *
	 * This method is used to select the yellow player character in the select character menu.
	 * It calls the SelectPlayer() function passing the YellowPlayerBP blueprint and "Yellow" as the parameters.
	 *
	 * @param void None
	 *
	 * @return void None
	 */
	UFUNCTION()
	void YellowPlayerSelect();

	/**
	 * @brief Selects the green player.
	 *
	 * This method is used to select the green player character in the Select Character Menu.
	 *
	 * @param GreenPlayerBP The blueprint of the green player character.
	 * @param GreenPlayerName The name of the green player character.
	 */
	UFUNCTION()
	void GreenPlayerSelect();

	/**
	 * @brief Selects the blue player.
	 *
	 * This method is responsible for selecting the blue player in the character menu.
	 * It internally calls the SelectPlayer() method to update the player selection.
	 *
	 * @param BluePlayerBP The blueprint of the blue player.
	 *
	 * @see SelectPlayer()
	 */
	UFUNCTION()
	void BluePlayerSelect();

	/**
	 * @brief Selects the black player character.
	 *
	 * This function is responsible for selecting the black player character.
	 * It calls the SelectPlayer() function from the USelectCharacterMenu class
	 * passing the black player blueprint as a parameter and "Black" as the player name.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION()
	void BlackPlayerSelect();

	/**
	 * @brief Updates the status of selected character buttons based on the current game state.
	 *
	 * This method checks the current game state and enables or disables the associated character buttons accordingly.
	 * If a player is already using a specific character, the button for that character will be disabled.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION()
	void UpdateSelectedCharacterButtons();

	/**
	 * Initializes the Select Character Menu. This method is called when the menu is created.
	 *
	 * @return True if the initialization was successful, otherwise false.
	 */
	virtual bool Initialize() override;
	
private:
	/**
	 *
	 */
	void SelectPlayer(TSubclassOf<APC_PlayerFox> PlayerBP, const FString& PlayerColorStr);

	/**
	 * @brief A variable representing a Pink Player Button.
	 *
	 * This variable is assigned to a UButton* property.
	 * It is marked with the meta attribute (BindWidget) to enable binding to a widget in Unreal Engine.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* PinkPlayerButton;

	/**
	 * @brief Pointer to the OrangePlayerButton widget.
	 *
	 * This variable is used to reference the OrangePlayerButton widget in Blueprint scripts.
	 * It is declared as a UPROPERTY to allow it to be properly managed by the Unreal Engine's garbage collector.
	 * The `meta = (BindWidget)` tag is used to automatically bind this variable to the corresponding widget in the
	 * User Interface (UI) Blueprint.
	 *
	 * @note Make sure to assign the widget reference in the Blueprint Editor or using code before accessing or
	 * using this variable. If the widget reference is not assigned, accessing this variable may result in a
	 * null pointer exception.
	 *
	 * @see UButton, BindWidget
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* OrangePlayerButton;

	/**
	 * @brief A UButton widget variable representing a yellow player button.
	 *
	 * This UButton widget is used to represent the yellow player button in the UI.
	 * It is bindable in the Blueprint Editor and can be used to handle button events.
	 *
	 * @see UButton
	 * @see UPROPERTY
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* YellowPlayerButton;

	/**
	 * @brief The GreenPlayerButton variable represents a reference to a UButton widget in Unreal Engine.
	 *
	 * @details This variable is declared as a UPROPERTY and is bound to a UButton widget in the Unreal Engine
	 * Editor. The meta attribute "BindWidget" is used to automatically bind this variable to a UButton widget
	 * with the same name.
	 *
	 * @note This variable should be assigned in the Unreal Engine Editor or programmatically using
	 * SetGreenPlayerButton() method.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* GreenPlayerButton;

	/**
	 * @brief BluePlayerButton
	 *
	 * This variable represents a reference to a UButton widget that is bound to the BluePlayerButton
	 * UPROPERTY in Unreal Engine.
	 *
	 * The BluePlayerButton widget is responsible for handling user input related to the blue player. This can
	 * include actions such as selecting the blue player, initiating an action for the blue player, or any other
	 * interaction specific to the blue player in the game.
	 *
	 * Use this variable to access and manipulate the BluePlayerButton widget in code, such as setting its
	 * visibility, handling click events, or updating its appearance.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* BluePlayerButton;

	/**
	 
	 * Represents a button widget that is binded to the BlackPlayerButton property.
	 
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* BlackPlayerButton;

	/**
	 * @brief CancelButton is a class variable of type UButton that represents the button used for canceling
	 * an action.
	 *
	 * @details The CancelButton is a property with metadata that binds it to a widget in the UI. It is used
	 * to provide a means for the user to cancel an action or close a window.
	 *
	 * @note This variable should be set in the Blueprint or Widget editor to ensure proper functionality.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	/**
	 *
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	/**
	 * @brief Takes the player back to the main menu.
	 *
	 * This method is a BlueprintCallable function that can be called to navigate the player back to the main menu.
	 * It checks if the MenuInterface is valid and calls the LoadMainMenu method on it. If the MenuInterface is null,
	 * an error log is printed indicating the failure to open the main menu.
	 *
	 * @return void
	 *
	 * @note This function should only be called when the player wants to go back to the main menu.
	 */
	UFUNCTION(BlueprintCallable)
	void BackToMainMenu();

	/**
	 * @brief Returns the gameplay to the main game.
	 *
	 * This method is used to resume the gameplay from the character selection menu.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void BackToGame();
	/**
	 * Writes a launch warning message on the screen for a player controller.
	 *
	 * @param PlayerController The player controller to write the warning for.
	 * @return true if the warning was successfully written, false otherwise.
	 */
	bool WriteLaunchWarningOnScreen(APlayerController* PlayerController);

	/**
	 * @brief Checks the start readiness delay for the game mode player controller.
	 *
	 * @param GameModePlayerController The game mode player controller to check the start readiness delay for.
	 */
	UFUNCTION(BlueprintCallable)
	static void CheckStartReadinessDelay(AGameModePlayerController* GameModePlayerController);

	/**
	 * @brief The delay timer for checking start readiness.
	 *
	 * The CheckStartReadinessDelayTimer variable is a float value that represents the delay in seconds for
	 * checking the start readiness. It is editable anywhere and its initial value is 2.0.
	 *
	 * @note Modifying this variable at runtime can affect the timing of the start readiness check.
	 */
	UPROPERTY(EditAnywhere)
	float CheckStartReadinessDelayTimer = 2.0;

	/**
	 * @brief Timer handle variable for delaying the start readiness check.
	 *
	 * Should be used to store the timer handle for delaying the
	 * start readiness check in Unreal Engine projects.
	 */
	FTimerHandle CheckStartReadinessDelayTimerHandle;

	/**
	 * @brief CheckStartReadinessDelayTimerDelegate represents a timer delegate used for checking the
	 * readiness delay.
	 *
	 * The CheckStartReadinessDelayTimerDelegate is used to handle a timer event that checks the readiness delay.
	 * It is used in conjunction with a timer object to execute a specific function or callback when the timer
	 * event is triggered.
	 *
	 * Usage example:
	 *
	 * ```cpp
	 * FTimerHandle TimerHandle;
	 * FTimerDelegate TimerDelegate;
	 *
	 * // Set up the timer delegate
	 * TimerDelegate.BindUObject(this, &AMyClass::OnTimerExpired);
	 *
	 * // Start the timer with the delegate
	 * GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 5.0f, false, 0.0f);
	 * ```
	 *
	 * @see FTimerHandle, FTimerDelegate
	 */
	FTimerDelegate CheckStartReadinessDelayTimerDelegate;

	/**
	 * @brief A map representing the relationship between character colors and buttons.
	 *
	 * This map stores character colors as keys and UButton pointers as values.
	 *
	 * @details
	 * The map enables easy access to buttons based on the corresponding character color.
	 *
	 * @note
	 * - The character colors are represented as FString objects.
	 * - The buttons are represented as UButton pointers.
	 *
	 * @see FString
	 * @see UButton
	 */
	std::map<FString, UButton*> CharacterColorButtonMap;
};
