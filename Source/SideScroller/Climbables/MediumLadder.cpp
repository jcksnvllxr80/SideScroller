// Fill out your copyright notice in the Description page of Project Settings.


#include "MediumLadder.h"

AMediumLadder::AMediumLadder()
{
	this->GetClimbableBox()->SetRelativeScale3D(FVector(0.25,0.4,1.0));
	this->GetClimbableBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}
