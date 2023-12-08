// Fill out your copyright notice in the Description page of Project Settings.


#include "SideScroller/Characters/Players/PC_PlayerGhost.h"

APC_PlayerGhost::APC_PlayerGhost()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APC_PlayerGhost::BeginPlay()
{
	Super::BeginPlay();
	if (NameBanner != nullptr)
	{
		NameBanner->SetVisibility(false);
	}
}
