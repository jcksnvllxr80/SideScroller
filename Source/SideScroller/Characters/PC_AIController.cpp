// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_AIController.h"

#include "BasePaperCharacter.h"
#include "Kismet/GameplayStatics.h"

APC_AIController::APC_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APC_AIController::DestroyActor()
{
	GetWorld()->GetTimerManager().ClearTimer(this->ShootTimerHandle);
}

void APC_AIController::CanShootAgain()
{
	this->CanShoot = true;
}

void APC_AIController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!PlayerPawn) return;
	// this->MoveToActor(PlayerPawn, 200.f);
	bool PlayerLOS = this->LineOfSightTo(PlayerPawn);
	MoveToActor(PlayerPawn, 100);
	if (PlayerLOS)
	{
		this->SetFocus(PlayerPawn);
		ABasePaperCharacter* EnemyAI = dynamic_cast<ABasePaperCharacter*>(this->GetPawn());
		if (EnemyAI)
		{
			if (this->CanShoot)
			{
				this->CanShoot = false;
				EnemyAI->AIShoot();
				GetWorld()->GetTimerManager().SetTimer(
					this->ShootTimerHandle,
					this,
					&APC_AIController::CanShootAgain,
					ShootDelayTime,
					false
				);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ABasePaperCharacter::Shoot cannot cast %s to EnemyPC."), *this->GetName());
		}
	}
	else
	{
		this->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

void APC_AIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}
