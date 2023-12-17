// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/Button.h"
#include "MainMenu.h"

/**
 * @brief Set up the server row.
 *
 * This method is responsible for setting up the server row with the given parent and index.
 *
 * @param InParent - The parent UMainMenu instance.
 * @param InIndex - The index of the server row.
 */
void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

/**
 * @brief Called when the server row is clicked.
 *
 * This method sets the selected index of the parent.
 */
void UServerRow::OnClicked()
{
	Parent->SetSelectedIndex(Index);
}
