// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
#include "SelectCharacterMenu.generated.h"

class APC_PlayerFox;
/**
 * 
 */
UCLASS()
class SIDESCROLLER_API USelectCharacterMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> PinkPlayerBP = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> OrangePlayerBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> YellowPlayerBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> GreenPlayerBP = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> BluePlayerBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> BlackPlayerBP = nullptr;

protected:
	UFUNCTION()
	void PinkPlayerSelect();
	
	UFUNCTION()
	void OrangePlayerSelect();
	
	UFUNCTION()
	void YellowPlayerSelect();
	
	UFUNCTION()
	void GreenPlayerSelect();
	
	UFUNCTION()
	void BluePlayerSelect();
	
	UFUNCTION()
	void BlackPlayerSelect();
	
	virtual bool Initialize() override;
	
private:
	void SelectPlayer(TSubclassOf<APC_PlayerFox> PlayerBP, const FString& PlayerColorStr);
	
	UPROPERTY(meta = (BindWidget))
	class UButton* PinkPlayerButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* OrangePlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* YellowPlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GreenPlayerButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BluePlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BlackPlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	
	UFUNCTION(BlueprintCallable)
	void BackToMainMenu();

	UFUNCTION(BlueprintCallable)
	virtual void BackToGame();
	bool WriteLaunchWarningOnScreen(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	static void CheckStartReadinessDelay(AGameModePlayerController* GameModePlayerController);

	UPROPERTY(EditAnywhere)
	float CheckStartReadinessDelayTimer = 2.0;
	
	FTimerHandle CheckStartReadinessDelayTimerHandle;
	
	FTimerDelegate CheckStartReadinessDelayTimerDelegate;
};
