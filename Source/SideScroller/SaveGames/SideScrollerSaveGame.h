// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SideScrollerSaveGame.generated.h"

/**
 * \brief This class represents the save game data for the side-scroller game.
 *
 * This class is derived from the USaveGame base class.
 *
 * The save game data includes the player's name, volume level, and resolution index.
 */
UCLASS()
class SIDESCROLLER_API USideScrollerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/**
	 * @var PlayerName
	 * @brief The name of the player.
	 *
	 * This variable stores the name of the player as a FString object.
	 * It is marked as a UPROPERTY to allow it to be accessible in blueprint scripts.
	 * The player name can be used to display the name of the player in the game or for other game logic.
	 */
	UPROPERTY()
	FString PlayerName;

	/**
	 * @brief The current volume level.
	 *
	 * This variable represents the current volume level. It is a float value, which can range from 0.0 to 1.0,
	 * where 0.0 represents mute and 1.0 represents the maximum volume level.
	 *
	 * @note This variable is marked with UPROPERTY, which indicates that it is a member of a Unreal Engine object
	 * and can be accessed and modified from various parts of the engine and editor.
	 *
	 * @see SetVolumeLevel(), GetVolumeLevel()
	 */
	UPROPERTY()
	float VolumeLevel;

	/**
	 * @brief The index of the current resolution.
	 *
	 * This variable represents the index of the current resolution. It is used in cases where there are multiple
	 * available resolutions, and this variable is used to determine the currently selected resolution.
	 *
	 * @note This variable is annotated with UPROPERTY(), indicating that it is a property within the Unreal Engine.
	 *       This annotation is used by the Unreal Engine for various purposes, such as serialization and replication.
	 */
	UPROPERTY()
	int ResolutionIndex;

	/**
	 * @brief Default constructor for USideScrollerSaveGame.
	 *
	 * @details This constructor initializes the USideScrollerSaveGame object with default values.
	 */
	USideScrollerSaveGame();
};
