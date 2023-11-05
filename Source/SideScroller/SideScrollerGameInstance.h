// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/MenuInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "GameModes/LobbyGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SidescrollerGameInstance.generated.h"

class USideScrollerSaveGame;
class APC_PlayerFox;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()
	FString ServerName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
	
};
/**
 * 
 */
UCLASS()
class SIDESCROLLER_API USideScrollerGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	USideScrollerGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;

	UFUNCTION(BlueprintCallable)
	void RefreshServerList() override;

	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void JoinIP(FString& IpAddress) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(BlueprintCallable)
	void SelectCharacterLoadMenu();

	UFUNCTION(BlueprintCallable)
	void GameOverLoadMenu();
	
	UFUNCTION(BlueprintCallable)
	void LoadGameOverMenu();

	UFUNCTION(BlueprintCallable)
	int GetNumPlayersToStartGame() const;
	
	UFUNCTION(BlueprintCallable)
	TSubclassOf<APC_PlayerFox> GetChosenCharacter(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SetChosenCharacter(APlayerController* PlayerController, TSubclassOf<APC_PlayerFox> ChosenCharacter);

	UFUNCTION(BlueprintCallable)
	bool IsEveryPlayersCharacterChosen() const;

	UFUNCTION(BlueprintCallable)
	void SetReadyToStartGame(bool bCond);

	UFUNCTION(BlueprintCallable)
	bool IsReadyToStartGame();

	UFUNCTION(BlueprintCallable)
	USideScrollerSaveGame* GetPlayerProfile() const;

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	
private:
	TSubclassOf<class UUserWidget> MainMenuClass = nullptr;
	TSubclassOf<class UUserWidget> InGameMenuClass = nullptr;
	TSubclassOf<class UUserWidget> SelectCharacterMenuClass = nullptr;
	TSubclassOf<class UUserWidget> SettingsMenuClass = nullptr;
	TSubclassOf<class UUserWidget> GameOverMenuClass = nullptr;
	class UMainMenu* Menu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> GameSessionSearch;
	int NumPlayers = 1;
	bool bReadyToStartGame = false;
	void OnGameSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	std::map<FString, TSubclassOf<APC_PlayerFox>> PlayerControllerChosenCharMap;
	FString DesiredServerName;
	void CreateSession();
	
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UPROPERTY()
	USideScrollerSaveGame* PlayerProfile;

	UPROPERTY()
	FString PlayerProfileSlot = "SideScrollerPlayerProfile";
};
