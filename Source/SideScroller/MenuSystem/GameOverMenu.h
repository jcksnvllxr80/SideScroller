// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "GameOverMenu.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API UGameOverMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()

	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitGame();
};
