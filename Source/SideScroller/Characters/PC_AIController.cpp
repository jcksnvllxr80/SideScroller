// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_AIController.h"

#include "BasePaperCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief Constructor for APC_AIController.
 *
 * This constructor initializes the PrimaryActorTick.bCanEverTick flag to true.
 */
APC_AIController::APC_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * This function is called when the game starts or when the Actor is spawned. It initializes the variables and
 * sets necessary references.
 *
 * @param none
 * @return none
 */
void APC_AIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

/**
 * This method is called every frame to update the AI controller's behavior.
 *
 * @param DeltaSeconds The time passed since the last tick, in seconds.
 */
void APC_AIController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!UpdateFocusPawn()) return;
	
	if (PlayerPawn == nullptr) return;
	this->MoveToActor(PlayerPawn, 200.f);

	FocusOnPawn();
}

/**
 * Updates the focus pawn for the AI controller.
 *
 * @return true if the focus pawn was successfully updated, false otherwise.
 */
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

/**
 * \brief Focuses on the player pawn and performs actions based on the LineOfSightTo(PlayerPawn)
 *
 * If the PlayerPawn is null, the method will return without performing any actions.
 * If the AI controller has a line of sight to the PlayerPawn, it will set the focus on the PlayerPawn and perform
 * additional actions. The additional actions include shooting if the AI can shoot and setting a timer to enable
 * shooting after a delay. If the AI controller is unable to cast itself as ABasePaperCharacter, a warning message
 * will be logged. If the AI controller does not have a line of sight to the PlayerPawn, the focus will be cleared.
 *
 * \param None
 *
 * \return None
 */
void APC_AIController::FocusOnPawn()
{
	if (PlayerPawn == nullptr) return;
	
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

/**
 * @brief Destroy the current actor.
 *
 * This method is responsible for destroying the current actor associated with the AI controller. It clears the shoot
 * timer handle before destroying the actor.
 *
 * @param None.
 * @return None.
 *
 */
void APC_AIController::DestroyActor()
{
	GetWorld()->GetTimerManager().ClearTimer(this->ShootTimerHandle);
}

/**
 * @brief Sets the ability to shoot again.
 *
 * This method sets the flag indicating whether the AI controller can shoot again.
 * By default, the AI controller is allowed to shoot, and this flag is set to true.
 * Calling this method will override the flag and set it to true.
 *
 * @param None
 * @return None
 */
void APC_AIController::CanShootAgain()
{
	this->CanShoot = true;
}
