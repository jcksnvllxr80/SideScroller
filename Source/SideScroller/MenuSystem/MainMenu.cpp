// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "ServerRow.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "SideScroller/SaveGames/SideScrollerSaveGame.h"

/**
 * Initializes the Main Menu.
 *
 * This method is called during the initialization of the Main Menu and sets up various button callbacks and
 * UI elements. It also loads the player data and sets the initial values for the custom player name, resolution
 * selection, and volume.
 *
 * @return True if the initialization is successful, False otherwise.
 */
bool UMainMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();

	LoadPlayerData();
	
	if (!SuccessfulInit) return false;
	
	if (OpenHostMenuButton)
	{
		OpenHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Host button during init."));
		return false;
	}
	
	if (OpenJoinMenuButton)
	{
		OpenJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Open Join Menu button during init."));
		return false;
	}

	if (OpenSettingsMenuButton)
	{
		OpenSettingsMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenSettingsMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Open Settings Menu button during init."));
		return false;
	}
	
	if (OpenProfileMenuButton)
	{
		OpenProfileMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenProfileMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Open Profile Menu button during init."));
		return false;
	}

	if (CustomPlayerName)
	{
		if (!PlayerProfile) return true;  // early return; player profile is null
		UE_LOG(LogTemp, Display, TEXT("UMainMenu::Initialize - PlayerName is set to %s."), *PlayerProfile->PlayerName);
		CustomPlayerName->SetText(FText::FromString(PlayerProfile->PlayerName));
		
		CustomPlayerName->OnTextCommitted.AddDynamic(this, &UMainMenu::SetCustomPlayerNameEnter);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UMainMenu::Initialize - Either CustomPlayerName or PlayerProfile is null.")
		);
		return false;
	}
	
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Join button during init."));
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Join button during init."));
		return false;
	}

	if (DesktopButton)
	{
		DesktopButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Desktop button during init."));
		return false;
	}

	if (BackButtonJoinMenu)
	{
		BackButtonJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Join menu's Back button during init."));
		return false;
	}
		
	if (BackButtonHostMenu)
	{
		BackButtonHostMenu->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Host menu's Back button during init."));
		return false;
	}

	if (BackButtonSettingsMenu)
	{
		BackButtonSettingsMenu->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Settings menu's Back button during init."));
		return false;
	}
		
	if (BackButtonProfileMenu)
	{
		BackButtonProfileMenu->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the Profile menu's Back button during init."));
		return false;
	}
	
	if (SetPlayerNameButton)
	{
		SetPlayerNameButton->OnClicked.AddDynamic(this, &UMainMenu::SetCustomPlayerName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the SetPlayerName button during init."));
		return false;
	}

	if (ResolutionSelectComboBox)
	{
		if (!PlayerProfile) return true;  // early return; player profile is null
		UE_LOG(LogTemp, Display,
			TEXT("UMainMenu::Initialize - ResolutionIndex is set to %i."),
			PlayerProfile->ResolutionIndex
		);
		ResolutionSelectComboBox->SetSelectedIndex(PlayerProfile->ResolutionIndex);

		ResolutionSelectComboBox->OnSelectionChanged.AddDynamic(this, &UMainMenu::SetResolution);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the ResolutionSelect combo-box during init."));
		return false;
	}

	if (VolumeSelectSlider)
	{
		if (!PlayerProfile) return true;  // early return; player profile is null
		UE_LOG(LogTemp, Display,
			TEXT("UMainMenu::Initialize - VolumeSelectSlider is set to %f."),
			PlayerProfile->VolumeLevel
		);
		VolumeSelectSlider->OnValueChanged.AddDynamic(this, &UMainMenu::SetVolume);

		VolumeSelectSlider->SetValue(PlayerProfile->VolumeLevel);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the VolumeSelect slider during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Main Menu Init complete!"));
	return true;
}

/**
 * UMainMenu constructor.
 *
 * Initializes the UMainMenu object and sets the ServerRowClass by finding the UUserWidget class using the provided
 * blueprint path.
 *
 * @param ObjectInitializer The object initializer reference.
 */
UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ServerRowBPClass.Class) return;
	ServerRowClass = ServerRowBPClass.Class;
	SetIsFocusable(true);
	// bIsFocusable = true;  // deprecated
}

/**
 * @brief LoadPlayerData method is responsible for loading player data from the game instance.
 *        It assigns the player profile to the member variable PlayerProfile.
 *
 * @param None
 *
 * @return None
 *
 * @note This method assumes that the game instance is of type USideScrollerGameInstance.
 *       If the game instance is null or the player profile is null, it logs appropriate errors.
 *       Make sure to check if the PlayerProfile is null after calling this method.
 */
void UMainMenu::LoadPlayerData()
{
	const USideScrollerGameInstance* GameInstance = Cast<USideScrollerGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UMainMenu::LoadPlayerData - Can't LoadPlayerData. GameInstance is null!")
		);
		return;
	}

	PlayerProfile = GameInstance->GetPlayerProfile();
	if (PlayerProfile == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UMainMenu::LoadPlayerData - Can't LoadPlayerData. PlayerProfile is null!")
		);
	}
}

/**
 * Adds available servers to the server list.
 *
 * @param ServersData The array of server data to add to the list.
 */
void UMainMenu::SetServerList(TArray<FServerData> ServersData)
{
	UE_LOG(LogTemp, Display, TEXT("Adding available servers to server list."))
	if (ServerRowClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Main Menu blueprint class %s."), *ServerRowClass->GetName());
		UWorld* World = this->GetWorld();
		if (World)
		{
			uint32 i = 0;
			ServerList->ClearChildren();
			for (const FServerData& ServerData : ServersData)
			{
				UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
				if (ServerRow)
				{
					if (ServerList)
					{
						ServerRow->ServerListItem->SetText(FText::FromString(ServerData.ServerName));
						ServerRow->HostUser->SetText(FText::FromString(ServerData.HostUserName));
						FString FractionText = FString::Printf(TEXT("%d/%d"),
							ServerData.CurrentPlayers, ServerData.MaxPlayers);
						ServerRow->ConnectionFraction->SetText(FText::FromString(FractionText));
						ServerRow->Setup(this, i);
						++i;
						
						ServerList->AddChild(ServerRow);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ServerList object is null."));
						return;
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Cant create widget ServerRow."));
					return;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant get World from this object."));
			return;
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the Main Menu blueprint class."));
		return;
	}
}

/**
 * @brief Get the number of players from the spinner
 *
 * @return The number of players selected
 */
int UMainMenu::GetNumberOfPlayers()
{
	return NumPlayersSpinBox->GetValue();
}

/**
 * \brief Retrieves the value of the volume slider.
 *
 * This method retrieves the current value of the volume slider.
 *
 * \return The current value of the volume slider.
 */
float UMainMenu::GetVolumeSliderValue()
{
	return VolumeSelectSlider->GetValue();
}

/**
 * This method is used to host a server.
 *
 * @param CustomServerName The name of the custom server to be hosted.
 *
 * @note This method requires an instance of the MenuInterface class to be set using the SetMenuInterface method
 * before it can be called.
 *
 * @note The CustomServerName parameter should contain the desired name of the server to be hosted.
 */
void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Display, TEXT("Hosting a server"))
	if (MenuInterface)
	{
		if (CustomServerName)
		{
			MenuInterface->Host(CustomServerName->GetText().ToString());
		}
	}
}

/**
 * Opens the host menu by setting the active widget of the menu switcher to the host menu.
 * If the menu switcher or the host menu is not found, an error message is logged and the method returns.
 *
 * @param None
 * @return None
 *
 * @see UWidgetSwitcher, UWidget
 */
void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher)
	{
		if (HostMenu)
		{
			MenuSwitcher->SetActiveWidget(HostMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OPEN HOST MENU FAILED! Cant find the HostMenu Widget."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OPEN HOST MENU FAILED! Cant find the Menu Switcher OBJ."));
		return;
	}
}

/**
 * Open the join menu.
 *
 * @param None.
 * @return None.
 */
void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher)
	{
		if (JoinMenu)
		{
			MenuSwitcher->SetActiveWidget(JoinMenu);
			if (MenuInterface)
			{
				MenuInterface->RefreshServerList();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("No Menu interface!"));
				return;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OPEN JOIN MENU FAILED! Cant find the JoinMenu Widget."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OPEN JOIN MENU FAILED! Cant find the Menu Switcher OBJ."));
		return;
	}
}

/**
 * OpenSettingsMenu function is responsible for switching to the SettingsMenu widget within the MainMenu.
 *
 * @param None
 * @return None
 */
void UMainMenu::OpenSettingsMenu()
{
	if (MenuSwitcher)
	{
		if (SettingsMenu)
		{
			MenuSwitcher->SetActiveWidget(SettingsMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenu::OpenSettingsMenu - Cant find the SettingsMenu Widget."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::OpenSettingsMenu - Cant find the Menu Switcher OBJ."));
		return;
	}
}

/**
 * Opens the profile menu.
 *
 * The profile menu is displayed by activating the ProfileMenu widget in the MenuSwitcher widget.
 * If either of these widgets is not found, an error message is logged and the method returns.
 *
 * @param None
 * @return None
 */
void UMainMenu::OpenProfileMenu()
{
	if (MenuSwitcher)
	{
		if (ProfileMenu)
		{
			MenuSwitcher->SetActiveWidget(ProfileMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMainMenu::OpenProfileMenu - Cant find the ProfileMenu Widget."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::OpenProfileMenu - Cant find the Menu Switcher OBJ."));
		return;
	}
}

/**
 * Joins a server either by IP address or server list item.
 *
 * @param IpAddress The IP address of the server to join. This parameter is optional and can be empty.
 * @param SelectedIndex The index of the server list item to join. This parameter is optional and can be unset.
 *
 * @note If the IP address is provided, it will take precedence over the selected index.
 * @note If neither the IP address nor the selected index is provided, the method will log an error and return without
 * joining a server.
 */
void UMainMenu::JoinServer()
{
	if (MenuInterface)
	{
        if (HostIpAddress)
        {
            FString IpAddress = HostIpAddress->GetText().ToString();
            if (!IpAddress.IsEmpty())
            {
                MenuInterface->JoinIP(IpAddress);	
            }
            else
            {
                UE_LOG(LogTemp, Display, TEXT("IP Address field is empty. Trying server list item."));
            	if (SelectedIndex.IsSet()) 
            	{
            		UE_LOG(LogTemp, Display, TEXT("Joining server index: %i"), SelectedIndex.GetValue());
            		MenuInterface->Join(SelectedIndex.GetValue());
            	}
            	else
            	{
            		UE_LOG(LogTemp, Error, TEXT("JOIN SERVER FAILED! Cant find a server to join."));
            		return;
            	}
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("JOIN SERVER FAILED! Cant find the HostIpAddress Widget."));
            return;
        }
	}
}

/**
 * @brief Quits the game.
 *
 * This method requests the game to exit. It first checks if a valid world is available. If a valid world is found,
 * it gets the first player controller. If a valid player controller is found, it sends the "quit" command to the
 * player controller's console. If any of the checks fail, an error message is logged and the method returns.
 *
 * @param None.
 *
 * @return None.
 */
void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			const FString QuitCommand = TEXT("quit");
			PlayerController->ConsoleCommand(QuitCommand);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find World."));
		return;
	}
}

/**
 * @brief Switches back to the main menu widget.
 *
 * This method is used to switch back to the main menu widget by making it the active widget in the menu switcher.
 * If the menu switcher or the main menu widget cannot be found, an error message is logged and the method returns.
 */
void UMainMenu::BackToMainMenu()
{
	if (MenuSwitcher)
	{
		if (MainMenu)
		{
			MenuSwitcher->SetActiveWidget(MainMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OPEN MAIN MENU FAILED! Cant find the MainMenu Widget."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OPEN MAIN MENU FAILED! Cant find the Menu Switcher OBJ."));
		return;
	}
}

/**
 * Retrieves the selected index from the main menu.
 *
 * @return The selected index as an optional unsigned 32-bit integer. If no index is selected, the optional
 * will be empty.
 */
TOptional<uint32> UMainMenu::GetSelectedIndex() const
{
	return SelectedIndex;
}

/**
 * Sets the selected index of the main menu.
 *
 * @param Index The index to set as selected. Use TOptional<uint32>() to clear the selected index.
 */
void UMainMenu::SetSelectedIndex(const TOptional<uint32>& Index)
{
	SelectedIndex = Index;
	UpdateChildrenRows();
}

/**
 * Updates the rows of the child widgets in the main menu.
 *
 * This method iterates through all the child widgets of the ServerList and updates their state based on the
 * selected index. If a child widget is of type UServerRow, it sets the "Selected" property to true if the
 * SelectedIndex is set and matches the child widget's index in the ServerList.
 *
 * @see UWidget
 * @see UServerRow
 */
void UMainMenu::UpdateChildrenRows()
{
	if (ServerList)
	{
		for (UWidget* CurrentWidget : ServerList->GetAllChildren())
		{
			auto Row = Cast<UServerRow>(CurrentWidget);
			if (Row)
			{
				Row->Selected = (SelectedIndex.IsSet() && (SelectedIndex.GetValue() == ServerList->GetChildIndex(Row)));
			}
		}
	}
}

/**
 * @brief Sets the custom player name entered by the user.
 *
 * This method is called when the user enters a custom player name.
 * It sets the custom player name using the SetCustomPlayerName() method.
 *
 * @param Text The text entered by the user as the custom player name.
 * @param CommitMethod The method used to commit the entered text.
 * @remark This method does not have a return value.
 */
void UMainMenu::SetCustomPlayerNameEnter(const FText& Text, ETextCommit::Type CommitMethod)
{
	SetCustomPlayerName();
}

/**
 * Sets the custom player name in the game instance's player profile and saves it to the profile SaveGame.
 * If there is no game instance, an error message is logged and the player name is not saved.
 *
 * @param None
 * @return None
 */
void UMainMenu::SetCustomPlayerName()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UMainMenu::SetCustomPlayerName - No GameInstance. Not saving player name to profile")
		);
		return;  // no game instance - early return
	}

	const FString PlayerNameText = CustomPlayerName->GetText().ToString();
	PlayerProfile->PlayerName = PlayerNameText;
	UE_LOG(LogTemp, Display,
		TEXT("UMainMenu::SetCustomPlayerName - Attempting to save PlayerName in profile SaveGame as %s"),
		*PlayerNameText
	);
	GameInstance->SaveGame();
}

/**
 * Sets the resolution of the game based on the selected item.
 *
 * @param SelectedItem The resolution selected from the menu.
 * @param SelectionType The type of selection made.
 */
void UMainMenu::SetResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	const std::map<FString, int> ResolutionMap = {
		{"640x480", 0},
		{"1280x720", 1},
		{"1920x1080", 2},
		{"2560x1440", 3}
	};
	
	FIntPoint Resolution;
	const int ResolutionIndex = ResolutionMap.find(SelectedItem)->second;
	switch (ResolutionIndex)
	{
	case 0: 
		Resolution.X = 640;
		Resolution.Y = 480;
		break;
	case 1:
		Resolution.X = 1280;
		Resolution.Y = 720;
		break;
	case 2: 
		Resolution.X = 1920;
		Resolution.Y = 1080;
		break;
	case 3: 
		Resolution.X = 2560;
		Resolution.Y = 1440;
		break; 
	default: 
		Resolution.X = 1280;
		Resolution.Y = 720;
		break; 
	}

	UE_LOG(LogTemp, Display,
		TEXT("UMainMenu::SetResolution - Changing resolution to %i x %i."),
		Resolution.X, Resolution.Y
	)

	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UMainMenu::SetResolution - No GameInstance. Not saving ResolutionIndex to profile")
		);
		return;  // no game instance - early return
	}

	GameInstance->GetEngine()->GameUserSettings->SetScreenResolution(Resolution);
	
	PlayerProfile->ResolutionIndex = ResolutionIndex;
	UE_LOG(LogTemp, Display,
		TEXT("UMainMenu::SetResolution - Attempting to save ResolutionIndex in profile SaveGame as %i"),
		ResolutionIndex
	);
	GameInstance->SaveGame();
}

/**
 * Sets the volume level.
 *
 * This method is responsible for setting the volume level. It logs the volume change to the console.
 * If there is no GameInstance, it logs an error and returns without saving the volume.
 * Otherwise, it updates the volume level in the PlayerProfile, logs the attempt to save the volume,
 * and calls the SaveGame method of the GameInstance to save the volume.
 *
 * @param Value The volume level to set.
 */
void UMainMenu::SetVolume(float Value)
{
	UE_LOG(LogTemp, Verbose, TEXT("UMainMenu::SetVolume - volume was changed to %f."), Value)

	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UMainMenu::SetVolume - No GameInstance. Not saving volume to profile")
		);
		return;  // no game instance - early return
	}

	PlayerProfile->VolumeLevel = Value;
	UE_LOG(LogTemp, Display,
		TEXT("UMainMenu::SetVolume - Attempting to save volume in profile SaveGame as %f"), Value
	);
	GameInstance->SaveGame();
}
