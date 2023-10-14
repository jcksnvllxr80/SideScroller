// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sidescroller/Interfaces/MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	void Setup();
	virtual void OnLevelRemovedFromWorld();

protected:
	IMenuInterface* MenuInterface;
};
