// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "GameCompleteCredits.generated.h"

/**
 * @class UGameCompleteCredits
 * @brief Represents a Game Complete Credits widget.
 *
 * This class inherits from UMenuWidget and provides functionality for a Game Complete Credits.
 */
UCLASS()
class SIDESCROLLER_API UGameCompleteCredits : public UMenuWidget
{
	GENERATED_BODY()

protected:
	/**
	 * Initializes the GameCompleteCredits.
	 *
	 * @return True if the initialization was successful, false otherwise.
	 */
	UFUNCTION()

	virtual bool Initialize() override;
	
private:
	/**
	 * @brief The reference to the main menu button widget.
	 *
	 * This variable represents the reference to the main menu button widget
	 * in the blueprint or C++ code. It is decorated with the UPROPERTY macro and
	 * the meta attribute "BindWidget" to enable binding with the corresponding UI
	 * element in the user interface editor or blueprint.
	 *
	 * Clients can use this variable to interact with the main menu button widget, such as
	 * adding event handlers, changing visibility, or updating properties.
	 *
	 * @see UButton
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	/**
	 * @brief Function called when the user selects the option to go back to the main menu.
	 *
	 * This function is called when the user selects the option to go back to the main menu from the
	 * Game Complete Credits. It checks if the MenuInterface is valid and then calls the LoadMainMenu function
	 * on the MenuInterface. If the MenuInterface is not valid, an error message is logged.
	 *
	 * @param None
	 *
	 * @return None
	 *
	 * @note This function assumes that the MenuInterface has been set correctly before calling this function.
	 */
	UFUNCTION()
	void BackToMainMenu();
};
