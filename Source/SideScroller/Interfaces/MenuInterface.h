// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

/**
 * @class UMenuInterface
 *
 * The UMenuInterface class is a minimal interface class used for menus.
 * It is a Blueprintable interface that can be implemented by any class
 * in order to provide menu-related functionality.
 *
 * To use this interface, implement it in your class using the GENERATED_BODY()
 * macro, and then override the desired functions defined in this interface.
 *
 * Example:
 * @code
 * // Create a new Blueprintable class and implement the UMenuInterface
 * UCLASS(Blueprintable)
 * class AMyMenuActor : public AActor, public UMenuInterface
 * {
 *     GENERATED_BODY()
 *
 * public:
 *     // Override the functions defined in UMenuInterface
 *     virtual void ShowMenu() override;
 *     virtual void HideMenu() override;
 * };
 * @endcode
 *
 * @see AActor
 */
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class IMenuInterface
 *
 * @brief Interface for menu functionality in a side scroller game.
 *
 * This class defines the interface functions that must be implemented by any class
 * that inherits from it. The interface functions provide functionality for hosting servers,
 * joining servers by IP address or index, loading the main menu, and refreshing the server list.
 */
class SIDESCROLLER_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief This method is used to host a game server with the specified server name.
	 *
	 * @param ServerName The name of the server to be hosted.
	 *
	 * @details This method is a pure virtual function that needs to be implemented in a derived class.
	 *          It is responsible for hosting a game server with the specified server name.
	 *          The implementation of this method will vary depending on the specific requirements of the derived class.
	 *
	 * @see DerivedClass
	 */
	virtual void Host(FString ServerName) = 0;
	/**
	 * @brief Joins the specified IP address.
	 *
	 * This method is used to join the specified IP address.
	 *
	 * @param IpAddress The IP address to be joined.
	 *
	 * @return None.
	 */
	virtual void JoinIP(FString& IpAddress) = 0;
	/**
	 * Joins the given index.
	 *
	 * This method takes in an unsigned integer index and performs a join operation on the specified index.
	 *
	 * @param Index The index to join.
	 *
	 * @remarks This method is a pure virtual function and must be implemented by derived classes.
	 */
	virtual void Join(uint32 Index) = 0;
	/**
	 * @brief Load the main menu.
	 *
	 * This method is called to load the main menu. It is a pure virtual method
	 * that needs to be implemented by the derived classes. The main menu is
	 * responsible for displaying the options available to the user and
	 * navigating to different sections of the program.
	 *
	 * This method does not have any parameters.
	 *
	 * Note: This method does not return anything.
	 */
	virtual void LoadMainMenu() = 0;
	/**
	 * @brief Refreshes the server list.
	 *
	 * This method is a pure virtual function that is used to refresh the server list.
	 * It does not return any values.
	 *
	 * @param None
	 *
	 * @return None
	 *
	 * @see None
	 */
	virtual void RefreshServerList() = 0;
};
