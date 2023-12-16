// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Components/Button.h"
#include "Sidescroller/SideScrollerGameInstance.h"
#include "SideScroller/SaveGames/SideScrollerSaveGame.h"
#include "MainMenu.generated.h"

/**
 * UMainMenu is a class that represents the main menu user interface*/
UCLASS()
class SIDESCROLLER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * UMainMenu is a class that represents the main menu user interface.
	 *
	 * UMainMenu constructor initializes the UMainMenu object.
	 *
	 * @param ObjectInitializer  The object initializer.
	 *
	 * @return  None.
	 */
	UMainMenu(const FObjectInitializer & ObjectInitializer);
	/**
	 * \brief Adds the available servers to the server list.
	 *
	 * This method takes an array of FServerData objects and adds the information about each server to the server list.
	 * It creates a UServerRow widget for each server and sets the server name, host user name, connection fraction,
	 * and sets up the widget to provide interaction with the server. The server list is cleared before adding the
	 * new servers.
	 *
	 * \param ServersData The array of FServerData objects representing the available servers.
	 *
	 * \return None.
	 *
	 * \see FServerData, UServerRow
	 */
	void SetServerList(TArray<FServerData> ServersData);
	/**
	 * @brief Returns the selected index, if any.
	 *
	 * This method returns the selected index, if any, from the main menu. The index represents
	 * the location of the selected item in the menu. If no item is currently selected, the method
	 * will return an empty optional value.
	 *
	 * @return An optional value representing the selected index, or an empty optional if no index is selected.
	 * @see TOptional
	 */
	TOptional<uint32> GetSelectedIndex() const;
	/**
	 * @brief Sets the selected index in the main menu.
	 *
	 * This method allows to set the selected index in the main menu. The selected index determines which menu option
	 * is currently selected.
	 *
	 * @param SelectedIndex The selected index value. Use TOptional<uint32>() to clear the selected index.
	 *
	 * @note Calling this method will also update the children rows in the main menu.
	 */
	void SetSelectedIndex(const TOptional<uint32>& SelectedIndex);

	/**
	 * @brief Get the number of players.
	 *
	 * This method retrieves the current number of players.
	 *
	 * @return The number of players.
	 *
	 * @remark This method is a BlueprintCallable and can be called from Blueprints.
	 */
	UFUNCTION(BlueprintCallable)
	int GetNumberOfPlayers();

	/**
	 * @brief Get the value of the volume slider.
	 *
	 * This method returns the current value of the volume slider.
	 *
	 * @return The value of the volume slider.
	 */
	UFUNCTION(BlueprintCallable)
	float GetVolumeSliderValue();

private:
	/**
	 * @brief Loads the player data from the game instance.
	 *
	 * This method retrieves the game instance and checks if it is a valid instance.
	 * If the game instance is null, an error message will be logged and the method will return.
	 *
	 * Then, it retrieves the player profile from the game instance. If the player profile is null,
	 * a warning message will be logged.
	 */
	void LoadPlayerData();

	/**
	 * @brief The class representing the server row in the user interface.
	 *
	 * This class is a subclass of UUserWidget and represents the visual representation of a server row
	 * in the user interface. It is used to display server information such as server name, current
	 * players, and ping.
	 *
	 * The class is defined by a TSubclassOf template, which allows selecting a specific subclass of
	 * UUserWidget as the server row class. It is initialized with nullptr by default.
	 */
	TSubclassOf<class UUserWidget> ServerRowClass = nullptr;

	/**
	 * @brief The HostButton variable.
	 *
	 * The HostButton variable is a pointer to a UButton object. It is decorated with UPROPERTY macro to support
	 * Unreal Engine's property system. The meta specifier (BindWidget) is used to indicate that this variable is
	 * a reference to a widget that should be bound to a widget in the User Interface (UI).
	 *
	 * The HostButton is used to represent a button widget in the UI that is responsible for initiating the hosting
	 * functionality. This button allows the user to host a game or perform some action related to hosting.
	 *
	 * @see UButton
	 * @see UProperty
	 * @see BindWidget
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	/**
	 * @brief The JoinButton variable represents a button widget used for joining a specific action.
	 *
	 * The JoinButton variable is an instance of the UButton class, which is a specialized widget class
	 * that serves as a visual representation of a button that can be clicked.
	 *
	 * Usage:
	 *   - The JoinButton can be used to trigger an action when clicked, such as joining a game, accepting
	 *     an invitation, or submitting a form.
	 *   - This variable is expected to be assigned a reference to a specific button widget in the user interface.
	 *   - The JoinButton should be bound to the actual widget in the designer or programmatically in the code.
	 *
	 * Developers should use this variable to handle interaction events or modify the appearance and behavior
	 * of the button, if necessary.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	/**
	 *
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenJoinMenuButton;

	/**
	 * @brief Variable representing a button to open the host menu
	 *
	 * This variable is a reference to a UButton object that is used to open the host menu.
	 * It is decorated with UPROPERTY macro and meta attribute BindWidget for Unreal Engine's reflection system.
	 *
	 * Usage:
	 * - Bind this variable to a button widget in the User Interface Designer.
	 * - Attach a function or event to this button to handle the open host menu action.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenHostMenuButton;

	/**
	 * @brief Button used to open the settings menu.
	 *
	 * This UButton variable represents a button widget that is used to open the settings menu.
	 * It is tagged with the UPROPERTY macro to bind it to a widget in the Unreal Engine editor.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenSettingsMenuButton;

	/**
	 * @brief This variable represents a button used to open the profile menu.
	 *
	 * The OpenProfileMenuButton is a UButton pointer that is bound to a widget using the UPROPERTY macro.
	 * It is used to reference a button widget that when clicked, opens the profile menu.
	 *
	 * Example Usage:
	 * @code
	 *     OpenProfileMenuButton->OnClicked.AddDynamic(this, &MyClass::OpenProfileMenu);
	 * @endcode
	 *
	 * @remark This variable must be assigned to a valid UButton widget using the editor or programmatically.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenProfileMenuButton;

	/**
	 * @brief The DesktopButton variable is a property that represents a button widget in the desktop UI.
	 *
	 * This variable is defined as a UButton pointer and is bound to a widget using the BindWidget meta attribute.
	 * The UButton class is a subclass of UWidget, which allows it to be placed in the UI and interacted with by
	 * the user. The DesktopButton widget can be used to perform various actions when clicked, such as opening a
	 * specific feature or screen.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* DesktopButton;

	/**
	 * @brief A variable representing a bindable widget named HostIpAddress.
	 *
	 * This variable is of type UEditableText and is used to bind to a text input widget.
	 * It can be used to get or set the IP address entered by the user.
	 *
	 * @note This variable is tagged with `meta = (BindWidget)` which means it is meant to be bound to a widget
	 * in the Unreal Engine editor.
	 *
	 * @see UEditableText
	 */
	UPROPERTY(meta = (BindWidget))
	class UEditableText* HostIpAddress;

	/**
	 * @brief Custom server name editable text.
	 *
	 * This variable is a UEditableText pointer that is used to bind a widget
	 * in the user interface to allow the user to enter a custom server name.
	 *
	 * @note This variable is a UPROPERTY and is tagged with `meta = (BindWidget)` to
	 *       enable binding the widget in the Unreal Engine.
	 */
	UPROPERTY(meta = (BindWidget))
	class UEditableText* CustomServerName;

	/**
	 * @brief This variable is a reference to a panel widget that represents the server list.
	 *
	 * The server list panel widget is used to display a list of available servers for clients to join.
	 * This variable is used to bind a UPanelWidget object to the ServerList variable in Blueprint.
	 *
	 * Example usage:
	 *   - Bind a panel widget to the ServerList variable in Blueprint:
	 *     ```cpp
	 *     UPROPERTY(meta = (BindWidget))
	 *     class UPanelWidget* ServerList;
	 *     ```
	 *   - Use the ServerList variable to perform operations on the server list panel widget.
	 */
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	/**
	 * @brief A variable representing a widget switcher for a menu.
	 *
	 * The variable MenuSwitcher is a UWidgetSwitcher pointer that is bound to a widget using the meta tag `BindWidget`.
	 * It can be used to switch between different menu widgets.
	 *
	 * @see UWidgetSwitcher
	 */
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	/**
	 * @brief The variable MainMenu represents a pointer to a UWidget object
	 *        which is used to reference the main menu widget in the code.
	 *
	 * The UPROPERTY macro is used to mark the variable as a Unreal Property, which
	 * allows it to be automatically serialized and replicated by the engine.
	 *
	 * The meta attribute (BindWidget) is used to specify that this variable is
	 * used to bind and reference a widget in the editor.
	 *
	 * By binding the widget, the MainMenu variable can be easily accessed and
	 * manipulated by other code objects.
	 */
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	/**
	 * @class HostMenu
	 * @brief Represents a widget used for hosting a menu.
	 *
	 * The HostMenu class is a UWidget-derived class that is used to display and interact with a menu in
	 * the user interface. It is decorated with the UPROPERTY macro, which specifies that the HostMenu member
	 * variable is bound to a widget in the Unreal Engine editor's user interface designer. The host menu widget
	 * can be accessed and manipulated through this member variable.
	 */
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	/**
	 * @brief The JoinMenu variable is a reference to a UWidget object that represents the join menu widget.
	 *
	 * The JoinMenu widget is used to join an existing game or create a new game session.
	 * It is bound to a widget in the UI and allows the player to input necessary information to join a game.
	 *
	 * @tparam UWidget The type of the widget object.
	 * @see UWidget
	 */
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	/**
	 * @class SettingsMenu
	 * @brief Class representing a settings menu widget in the game.
	 *
	 * This class represents a settings menu widget in the game. It is a UserWidget
	 * and derived from the UWidget class. It is bound to a UWidget* property to
	 * allow easy access and manipulation in the game code.
	 *
	 * The settings menu is used to display and modify various settings related to
	 * the game's audio, video, controls, and other configurations.
	 */
	UPROPERTY(meta = (BindWidget))
	class UWidget* SettingsMenu;

	/**
	 * @brief Profile menu widget pointer.
	 *
	 * This variable holds a pointer to the profile menu widget. It is annotated with the UPROPERTY macro
	 * because it is bound to a widget in the user interface. The widget is declared as a UWidget pointer
	 * named 'ProfileMenu'.
	 */
	UPROPERTY(meta = (BindWidget))
	class UWidget* ProfileMenu;

	/**
	 * @brief Holds a reference to the back button in the host menu widget.
	 *
	 * This variable is used to bind to the back button widget in the host menu.
	 * The back button is typically used to navigate back to the previous menu or screen.
	 * The widget is of type UButton* and is annotated with the BindWidget meta attribute.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonHostMenu;

	/**
	 * @brief A property representing a button widget for returning to the previous menu in a join menu.
	 *
	 * This button widget is used for navigating back to the previous menu in a join menu.
	 * It is typically bound to a UButton object in Unreal Engine.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonJoinMenu;

	/**
	 * @brief Back button for the settings menu.
	 *
	 * This variable represents the back button widget used in the settings menu.
	 * It is binded to a UButton class to handle click events.
	 * The BackButtonSettingsMenu is used to navigate back to the previous menu or screen.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonSettingsMenu;

	/**
	 * @brief A UButton variable representing the back button in the profile menu.
	 *
	 * This UButton variable is used to reference the back button in the profile menu.
	 * It is bound to the UButton widget in the user interface, allowing for interaction
	 * and navigation within the profile menu.
	 *
	 * @note This variable is declared using Unreal Engine's UPROPERTY macro, which provides
	 * additional functionalities such as reflection and serialization.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonProfileMenu;

	/**
	 * @brief SetPlayerNameButton is a property that represents a button widget within a class.
	 *
	 * This property is designed to be bound to a button widget in the Unreal Engine user interface.
	 * It allows a user to set the name of a player by triggering a button click event.
	 *
	 * @see UButton
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* SetPlayerNameButton;

	/**
	 * \brief Variable representing a spin box widget for selecting the number of players.
	 *
	 * This variable is a UPROPERTY that is bound to a USpinBox widget. It can be used to retrieve and manipulate
	 * the selected value of the spin box.
	 *
	 * Example usage:
	 * \code{.cpp}
	 *   // Get the value of the spin box
	 *   int32 NumPlayers = NumPlayersSpinBox->GetValue();
	 *
	 *   // Set the value of the spin box
	 *   NumPlayersSpinBox->SetValue(4);
	 * \endcode
	 *
	 * \remarks
	 * The USpinBox widget must be properly set up and assigned to this variable before accessing its value or
	 * manipulating it.
	 *
	 * \see https://docs.unrealengine.com/en-US/API/Runtime/UMG/Components/USpinBox/index.html
	 */
	UPROPERTY(meta = (BindWidget))
	class USpinBox* NumPlayersSpinBox;

	/**
	 * @brief CustomPlayerName is a variable of type UEditableText* used to store a reference to a widget class
	 * named UEditableText in a Unreal Engine UPROPERTY.
	 *
	 * This widget class represents an editable text input field that allows the player to enter their own custom
	 * name. The UPROPERTY macro with the meta BindWidget tag is used to bind the UEditableText widget to this
	 * variable in the Unreal Engine editor. This enables the variable to be accessed and modified through
	 * Blueprint scripts or C++ code.
	 */
	UPROPERTY(meta = (BindWidget))
	class UEditableText* CustomPlayerName;

	/**
	 * @brief The ResolutionSelectComboBox class represents a ComboBox widget used for selecting resolutions.
	 *
	 * This class defines a UComboBoxString variable named ResolutionSelectComboBox which is used
	 * as a widget in the user interface for selecting resolutions. The @UPROPERTY macro is used to
	 * bind the widget to the variable.
	 */
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ResolutionSelectComboBox;

	/**
	 * @brief The VolumeSelectSlider class represents a slider widget used for selecting volume.
	 *
	 * The VolumeSelectSlider widget is a USlider object that is used to select the volume level.
	 * It is typically bound to a visual slider element in the UI.
	 */
	UPROPERTY(meta = (BindWidget))
	class USlider* VolumeSelectSlider;

	/**
	 * @brief The PlayerProfile variable stores the instance of the player's save game data.
	 */
	UPROPERTY()
	USideScrollerSaveGame* PlayerProfile;

	/**
	 * @brief Hosts a server with a given server name.
	 *
	 * This method is used to host a server with a custom server name. It sends the server name to the MenuInterface object
	 * for further processing.
	 *
	 * @param CustomServerName The custom server name entered by the user.
	 *
	 * @note This method assumes that the MenuInterface object has been assigned a valid value before calling this method.
	 * @note If the CustomServerName is empty or null, the hosting will not proceed.
	 */
	UFUNCTION()
	void HostServer();

	/**
	 * Open the host menu.
	 *
	 * This method is called to open the host menu widget. It activates the host menu widget by setting it as the
	 * active widget in the menu switcher. If the host menu widget is not found, an error message is logged
	 * and the method returns. If the menu switcher object is not found, an error message is logged and
	 * the method returns.
	 *
	 * @param None.
	 * @return None.
	 */
	UFUNCTION()
	void OpenHostMenu();

	/**
	 * Opens the join menu.
	 *
	 * This method sets the active widget to the join menu widget in the menu switcher, and calls the
	 * RefreshServerList method on the menu interface. If the menu interface is not found, an error message
	 * is logged and the method returns. If the join menu widget or the menu switcher object is not found,
	 * an error message is logged and the method returns.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION()
	void OpenJoinMenu();

	/**
	 * This method is used to open the settings menu.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION()
	void OpenSettingsMenu();

	/**
	 * Opens the profile menu.
	 *
	 * This method is called to open the profile menu widget. It sets the active widget of the menu switcher
	 * to the profile menu widget. If the menu switcher or profile menu widget is not found, an error message
	 * is logged and the method returns.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION()
	void OpenProfileMenu();

	/**
	 * @brief Joins a server using either an IP address or a selected server index.
	 *
	 * This method is called when the user attempts to join a server in the main menu. If an IP address is
	 * provided in the HostIpAddress widget, it will call the MenuInterface->JoinIP() function with the provided
	 * IP address. If the IP address field is empty, it will attempt to join a server using the selected server
	 * index from the server list. If no server index is selected, it will log an error and return.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION()
	void JoinServer();

	/**
	 * Quits the game by sending a 'quit' console command to the player controller.
	 *
	 * @param  None
	 * @return None
	 */
	UFUNCTION()
	void QuitGame();

	/**
	 * @brief Switches the active widget to the main menu.
	 *
	 * This method checks if the MenuSwitcher and MainMenu widgets are valid. If both widgets are valid, it
	 * sets the active widget in the MenuSwitcher to the MainMenu widget. If the MainMenu widget is not valid, an
	 * error message is logged and the method returns. If the MenuSwitcher widget is not valid, an error message is
	 * logged and the method returns.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION()
	void BackToMainMenu();

	/**
	 * @brief Sets the custom player name in the game.
	 *
	 * This method is responsible for setting the custom player name entered by the player in the game. It
	 * retrieves the game instance, checks for its availability, and saves the player name to the player's
	 * profile. It also logs error messages if the game instance is not found or if any errors occur during
	 * the process of saving the player name.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION()
	void SetCustomPlayerName();

	/**
	 * Sets the custom player name when the user enters the text and commits it.
	 *
	 * @param Text The entered text.
	 * @param CommitMethod The commit method used to enter the text.
	 */
	UFUNCTION()
	void SetCustomPlayerNameEnter(const FText& Text, ETextCommit::Type CommitMethod);


	/**
	 * Sets the screen resolution based on the selected item from the dropdown menu.
	 *
	 * @param SelectedItem - The selected resolution item as a string.
	 * @param SelectionType - The type of selection made.
	 */
	UFUNCTION()
	void SetResolution(FString SelectedItem, ESelectInfo::Type SelectionType);

	/**
	 * Sets the volume level.
	 *
	 * This method sets the volume level and saves it to the player's profile using the SaveGame function
	 * in the GameInstance. If there is no GameInstance available, it will log an error message and exit
	 * early without saving the volume level.
	 *
	 * @param Value The volume level to set.
	 */
	UFUNCTION()
	void SetVolume(float Value);

	/**
	 * @brief Represents the selected index value.
	 *
	 * The SelectedIndex variable is an optional unsigned 32-bit integer value
	 * that represents the index of a selected item in a collection or list.
	 *
	 * The value of SelectedIndex will be None if no item is selected.
	 *
	 * @see TOptional
	 *
	 * @note This documentation is generated using Doxygen.
	 */
	TOptional<uint32> SelectedIndex;

	/**
	 * @brief Updates the rows of the child widgets in the server list.
	 *
	 * This method iterates through each child widget in the server list and updates its row based on the
	 * selected index value. If the selected index is set and matches the index of the current widget in the
	 * server list, the widget's Selected property is set to true.
	 */
	void UpdateChildrenRows();
	
protected:
	/**
	 * Initializes the main menu.
	 *
	 * This method initializes the main menu by loading player data and setting up event bindings for various
	 * UI elements. It also sets the custom player name text and handles the user input for setting the player name.
	 *
	 * @return true if initialization is successful, false otherwise
	 */
	virtual bool Initialize() override;
};
