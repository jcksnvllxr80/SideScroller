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

bool UMainMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
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
		ResolutionSelectComboBox->OnSelectionChanged.AddDynamic(this, &UMainMenu::SetResolution);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the ResolutionSelect combo-box during init."));
		return false;
	}

	if (VolumeSelectSlider)
	{
		VolumeSelectSlider->OnValueChanged.AddDynamic(this, &UMainMenu::SetVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainMenu::Initialize - Cant find the VolumeSelect slider during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Main Menu Init complete!"));
	return true;
}

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ServerRowBPClass.Class) return;
	ServerRowClass = ServerRowBPClass.Class;
	bIsFocusable = true;
}

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

int UMainMenu::GetNumberOfPlayers()
{
	return NumPlayersSpinBox->GetValue();
}

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

TOptional<uint32> UMainMenu::GetSelectedIndex() const
{
	return SelectedIndex;
}

void UMainMenu::SetSelectedIndex(const TOptional<uint32>& Index)
{
	SelectedIndex = Index;
	UpdateChildrenRows();
}

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

void UMainMenu::SetCustomPlayerName()
{
	// TODO: write the SetCustomPlayerName function body
}

void UMainMenu::SetResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// TODO: write the SetResolution function body
}

void UMainMenu::SetVolume(float Value)
{
	// TODO: write the SetVolume function body
}
