// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "SelectCharacterMenu.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API USelectCharacterMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APawn> PinkPlayerBP = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APawn> OrangePlayerBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APawn> YellowPlayerBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APawn> GreenPlayerBP = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APawn> BluePlayerBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APawn> BlackPlayerBP = nullptr;

protected:
	void SelectPlayer(TSubclassOf<APawn> PlayerBP, const FString& PlayerColorStr) const;
	
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
	
	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void BackToGame();
	
};
