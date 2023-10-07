// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/MenuInterface.h"
#include "SideScrollerGameInstance.generated.h"

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
	// virtual void LoadMainMenu() override;
	
	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	void PlayGame() override;
	
private:
	TSubclassOf<class UUserWidget> MainMenuClass = nullptr;
	TSubclassOf<class UUserWidget> InGameMenuClass = nullptr;
	TSubclassOf<class UUserWidget> SettingsMenuClass = nullptr;
};
