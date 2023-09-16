// Fill out your copyright notice in the Description page of Project Settings.


#include "LargeLadder.h"

ALargeLadder::ALargeLadder()
{
	this->GetClimbableBox()->SetRelativeScale3D(FVector(0.25,0.4,2.0));
	this->GetClimbableBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}
