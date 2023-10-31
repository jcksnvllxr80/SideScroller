// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Components/Button.h"
#include "Sidescroller/SideScrollerGameInstance.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);
	void SetServerList(TArray<FServerData> ServersData);
	TOptional<uint32> GetSelectedIndex() const;
	void SetSelectedIndex(const TOptional<uint32>& SelectedIndex);

	UFUNCTION(BlueprintCallable)
	int GetNumberOfPlayers();
	
private:
	TSubclassOf<class UUserWidget> ServerRowClass = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenSettingsMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenProfileMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DesktopButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* HostIpAddress;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* CustomServerName;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* SettingsMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* ProfileMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonHostMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonJoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonSettingsMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButtonProfileMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* SetPlayerNameButton;

	UPROPERTY(meta = (BindWidget))
	class USpinBox* NumPlayersSpinBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* CustomPlayerName;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ResolutionSelectComboBox;

	UPROPERTY(meta = (BindWidget))
	class USlider* VolumeSelectSlider;
	
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenHostMenu();
	
	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenSettingsMenu();

	UFUNCTION()
	void OpenProfileMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void SetCustomPlayerName();

	UFUNCTION()
	void SetResolution(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void SetVolume(float Value);

	TOptional<uint32> SelectedIndex;

	void UpdateChildrenRows();

protected:
	virtual bool Initialize() override;
};
