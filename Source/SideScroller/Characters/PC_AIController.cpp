// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_AIController.h"

#include "BasePaperCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

APC_AIController::APC_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APC_AIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void APC_AIController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!UpdateFocusPawn()) return;
	
	if (PlayerPawn == nullptr) return;
	this->MoveToActor(PlayerPawn, 200.f);

	FocusOnPawn();
}

bool APC_AIController::UpdateFocusPawn()
{
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_AIController::UpdateFocusPawn - No update. Cant find World."));
		return false;
	}
	
	AGameStateBase* GameState = World->GetGameState();
	if (GameState == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_AIController::UpdateFocusPawn - No update. Cant find GameState."));
		return false;
	}

	APawn* ClosestPlayerPawn = this->PlayerPawn;
	for (APlayerState* PlayerStateI : GameState->PlayerArray)
	{
		if (ClosestPlayerPawn == nullptr)
		{
			ClosestPlayerPawn = PlayerStateI->GetPawn();
		}
		else
		{
			if (this->PlayerPawn == PlayerStateI->GetPawn())
			{
				continue;  // dont need to compare our own distance
			}
		
			const APawn* AIPawn = this->GetPawn();
			if (AIPawn == nullptr)
			{
				UE_LOG(
					LogTemp, Warning, TEXT("APC_AIController::UpdateFocusPawn - No update. %s has no pawn."),
					*this->GetName()
				);
				return false;
			}
			
			if (
				AIPawn->GetDistanceTo(this->PlayerPawn) > AIPawn->GetDistanceTo(PlayerStateI->GetPawn())
			) {
				UE_LOG(LogTemp, Display,
					TEXT("APC_AIController::UpdateFocusPawn - UpdatingAI, %s, focus pawn to Player: %s."),
					*this->GetName(),
					*PlayerStateI->GetName()
				);
				this->PlayerPawn = PlayerStateI->GetPawn();
			}
		}
	}
	return true;
}

void APC_AIController::FocusOnPawn()
{
	if (this->LineOfSightTo(PlayerPawn))
	{
		this->SetFocus(PlayerPawn);
		
		ABasePaperCharacter* EnemyAI = UECasts_Private::DynamicCast<ABasePaperCharacter*>(this->GetPawn());
		if (EnemyAI)
		{
			if (this->CanShoot)
			{
				EnemyAI->Shoot();
				this->CanShoot = false;

				GetWorld()->GetTimerManager().SetTimer(
					this->ShootTimerHandle,
					this,
					&APC_AIController::CanShootAgain,
					EnemyAI->GetShootDelayTime(),
					false
				);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("APC_AIController::FocusOnPawn - cannot cast %s to EnemyPC."),
			*this->GetName());
		}
	}
	else
	{
		this->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

void APC_AIController::DestroyActor()
{
	GetWorld()->GetTimerManager().ClearTimer(this->ShootTimerHandle);
}

void APC_AIController::CanShootAgain()
{
	this->CanShoot = true;
}
