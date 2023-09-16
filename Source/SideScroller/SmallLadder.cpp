// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallLadder.h"

ASmallLadder::ASmallLadder()
{
	this->GetClimbableBox()->SetRelativeScale3D(FVector(0.25,0.4,0.5));
	this->GetClimbableBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}
