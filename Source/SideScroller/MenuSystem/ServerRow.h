// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * @class UServerRow
 *
 * @brief The UServerRow class represents a server list item in the UI.
 *
 * UServerRow is a subclass of UUserWidget and provides functionality for displaying server information in the
 * server list. It contains UTextBlock widgets for displaying server details like the host user and
 * connection fraction.
 *
 * Usage:
 * - Create an instance of UServerRow.
 * - Call the Setup function to initialize the server row with the appropriate parent and index values.
 * - Bind the OnClicked event of the RowButton widget to the OnClicked function of the UServerRow.
 */
UCLASS()
class SIDESCROLLER_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief A variable representing a server list item.
	 *
	 * This variable is a reference to a UTextBlock widget that represents a server list item.
	 * The UTextBlock widget is used to display information about a server in the server list.
	 *
	 * @see UTextBlock
	 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerListItem;

	/**
	 * @brief The HostUser variable represents a bound UTextBlock widget.
	 *
	 * This variable is used to display the host user information in the UI.
	 *
	 * - Meta: BindWidget
	 * - Type: UTextBlock*
	 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;

	/**
	 * @brief Representation of the connection fraction.
	 *
	 * This variable represents a connection fraction and is used to bind a UTextBlock widget.
	 * The widget will display the connection fraction, indicating the current state of the connection.
	 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectionFraction;

	/**
	 * @brief Variable indicating whether an object is selected or not.
	 *
	 * This variable is used to store the selection status of an object. The value is
	 * set to true if the object is selected, and false otherwise.
	 *
	 * @note This variable is marked as BlueprintReadOnly, which means it is read-only in
	 * blueprint classes.
	 *
	 * @see UObject, UPROPERTY
	 */
	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	/**
	 * \brief Setup function for the Server Row
	 * \param InParent The parent menu
	 * \param InIndex The index value
	 *
	 * This function sets up the Server Row with the given parameters. It assigns the InParent and InIndex values to
	 * the corresponding member variables. It also binds the OnClicked event of the RowButton to the
	 * UServerRow::OnClicked function.
	 */
	void Setup(class UMainMenu* InParent, uint32 InIndex);

private:
	/**
	 * @brief Variable declaration for RowButton
	 *
	 * This variable represents a UButton widget. It is declared as a UPROPERTY, with the meta attribute
	 * set to (BindWidget). This allows the variable to be bound to a widget in the UMG editor.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	/**
	 * @brief Parent variable declaration.
	 *
	 * Parent is a pointer to the UMainMenu class, a representation of the main menu in the game.
	 * It is declared as a UPROPERTY, indicating that it can be accessed and modified by the Unreal Engine's
	 * reflection system.
	 *
	 * This variable is used to reference the parent main menu instance from submenus or other related classes,
	 * providing a way to access and manipulate its properties and functions.
	 *
	 * @note Make sure the UMainMenu class has been properly defined and is accessible from the current scope.
	 */
	UPROPERTY()
	class UMainMenu* Parent;

	/**
	 * @brief Index variable
	 *
	 * This variable represents an unsigned 32-bit integer Index.
	 * It can be used to store an index value or to keep track of an index value within a specific range.
	 *
	 * @note Make sure to initialize the Index variable before using it to avoid undefined behavior.
	 */
	uint32 Index;

	/**
	 * @brief Function called when the server row is clicked.
	 *
	 * This function sets the selected index of the parent object to the current index.
	 * The parent object is assumed to have a method called SetSelectedIndex with an integer parameter.
	 */
	UFUNCTION()
	void OnClicked();
};
