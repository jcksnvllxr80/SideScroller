// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "RespawnMenu.generated.h"

UCLASS()
class SIDESCROLLER_API URespawnMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void RunRespawnTimer();
	
	UFUNCTION()
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RespawnButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	
	FTimerHandle RespawnDelayTimerHandle;

	UPROPERTY()
	float RespawnDelayTime = 2.f;

	UFUNCTION()
	void Respawn();
	
	UFUNCTION()
	void BackToMainMenu();
	
	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void EnableRespawnButton() const;
};
