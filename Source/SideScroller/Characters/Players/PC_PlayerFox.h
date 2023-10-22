#pragma once
#include "CoreMinimal.h"
#include "SideScroller/Climbables/BaseClimbable.h"
#include "SideScroller/Characters/BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"
#include "PC_PlayerFox.generated.h"


class ASideScrollerGameModeBase;
class USideScrollerGameInstance;

UCLASS()
class SIDESCROLLER_API APC_PlayerFox : public ABasePaperCharacter, public IProjectileInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetAccumulatedPoints() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetAccumulatedPoints(const int Points);

	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetNumberOfLives() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetNumberOfLives(const int NumberOfLives);
	
	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetCherryCount() const;
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetCherryStash(int CherryStash);

	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetMoneyCount() const;
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetMoneyStash(const int MoneyAmount);
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	FText GetSpectatorsAsStr() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	FText GetPlayerName() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetSpectatorsStr();
	
	UFUNCTION(BlueprintCallable, Category = "Spectators")
	TArray<APC_PlayerFox*> GetSpectators() const;

	UFUNCTION(BlueprintCallable, Category = "Spectators")
	void AddToSpectators(APC_PlayerFox* Spectator);

	UFUNCTION(BlueprintCallable, Category = "Spectators")
	void RemoveFromSpectators(APC_PlayerFox* Spectator);

	UFUNCTION(BlueprintCallable, Category = "Death")
	void PlayerDeath();

	UFUNCTION(BlueprintCallable, Category = "Fall")
	void HandleFallOffLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* FallAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* CrouchAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* ClimbAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* StopOnLadderAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideAngleDeg = 15.f;


	UFUNCTION(BlueprintCallable)
	void TakeMoney(int MonetaryValue);

	UFUNCTION(BlueprintCallable)
	void TakeHealing(float HealingValue);

	UFUNCTION(BlueprintCallable)
	void TakeCherries(int NumCherries);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APC_PlayerFox();

	UFUNCTION(BlueprintCallable)
	void SpectateNextPlayer();

	UFUNCTION(BlueprintCallable)
	void Spectate() const;

	UFUNCTION(BlueprintCallable)
	void SpectatePrevPlayer();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Jump() override;
	
	UFUNCTION(BlueprintCallable)
	void CrouchClimbDown();

	UFUNCTION(BlueprintCallable)
	void StopCrouchClimb();

	UFUNCTION(BlueprintCallable)
	void ClimbUp();

	UFUNCTION(BlueprintCallable)
	void StopClimb();

	UFUNCTION(BlueprintCallable)
	void SetOverlappingClimbable(bool bOverlappingClimbable, ABaseClimbable* OverlappedClimbable);

	UFUNCTION(BlueprintCallable)
	virtual void SetProjectileTransform(
		const float Direction,
		AActor* MyOwner,
		const ABasePaperCharacter* BaseChar,
		const ABaseProjectile* Projectile
	) override;

	UFUNCTION(BlueprintCallable)
	void SetLastCheckpointLocation(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	void BeginSpectating(const ASideScrollerGameModeBase* GameMode, bool SearchInReverse);

	UFUNCTION(BlueprintCallable)
	void SpectateOtherPlayer();

	UFUNCTION(BlueprintCallable)
	void ReviveAtCheckpoint();
	
	UFUNCTION(BlueprintCallable)
	void DeathCleanUp();

	UFUNCTION(BlueprintCallable)
	void PrintPlayersList(TArray<APC_PlayerFox*> PlayersArray);

private:
	USideScrollerGameInstance* GameInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetPlayerHUD;

	UPROPERTY(EditAnywhere, replicated)
	UUserWidget* WidgetPlayerHUDInstance;
	
	UPROPERTY(EditAnywhere)
	FVector LastCheckpointLocation;

	UPROPERTY(EditAnywhere)
	int NumberOfLives = 5;

	UPROPERTY(EditAnywhere)
	int AccumulatedPoints = 0;

	UPROPERTY(EditAnywhere)
	float ClimbSpeed = 0.17f;

	UPROPERTY(EditAnywhere)
	float ClimbingLateralSpeed = 0.4;

	UPROPERTY(EditAnywhere)
	float CrouchSlideFriction = 0.175;

	float StandingFriction;

	bool bIsOutOfLives = false;

	float NormalWalkingSpeed;

	UPROPERTY(EditAnywhere, replicated)
	float MaxRunningSpeed = 500.f;
	
	UPROPERTY(EditAnywhere)
	double CrouchSlidingThresholdVelocity = 200.f;

	UPROPERTY(EditAnywhere)
	int CherryStash = 0;

	UPROPERTY(EditAnywhere)
	int MoneyStash = 0;
	
	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere)
	USoundBase* WalkSound;

	UPROPERTY(EditAnywhere)
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere)
	bool bIsClimbing = false;

	UPROPERTY(EditAnywhere)
	bool bIsSliding = false;

	UPROPERTY(EditAnywhere)
	bool bOnLadder = false;

	UPROPERTY(EditAnywhere, replicated)
	TArray<APC_PlayerFox*> Spectators;

	UPROPERTY(EditAnywhere, replicated)
	APC_PlayerFox* PlayerBeingSpectated;
	
	UPROPERTY(EditAnywhere)
	bool bOverlappingClimbable;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	USoundBase* NearbyClimbableSound;

	UPROPERTY(EditAnywhere)
	float CumulativeTime = 0.f;

	UFUNCTION(BlueprintCallable)
	void MoveSpectatorsToNewPlayer() const;
	
	UFUNCTION(BlueprintCallable)
	bool FoundPlayerToSpectate(APC_PlayerFox* Player);
	
	UFUNCTION(BlueprintCallable)
	void ClimbUpAxisInputCallback(float Z);
	
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Axis);

	UFUNCTION(BlueprintCallable)
	void Climb(float Value);

	UFUNCTION(BlueprintCallable)
	void LogSpeed();

	UFUNCTION(BlueprintCallable)
	void LogRotation();

	UFUNCTION(BlueprintCallable)
	void SetRunVelocity();

	UFUNCTION(BlueprintCallable)
	void SetWalkVelocity();
	
	UFUNCTION(BlueprintCallable)
	void LogLocation();

	UFUNCTION(BlueprintCallable)
	void OpenInGameMenu();

	UFUNCTION(BlueprintCallable)
	void DoWalkAnimAndSound();

	UFUNCTION(BlueprintCallable)
	void DoClimbAnimAndSound();

	UFUNCTION(BlueprintCallable)
	void AddToPlayersArray();

	UFUNCTION(BlueprintCallable)
	void RemoveFromPlayersArray();

	UFUNCTION(BlueprintCallable)
	void PlayerHUDSetup();

	UFUNCTION(BlueprintCallable)
	void PlayerHUDTeardown();

	UPROPERTY(EditAnywhere)
	FVector CrouchProjectileSpawnPoint = FVector(0.f, 0.f, 5.f);

	UPROPERTY(EditAnywhere)
	FVector ProjectileUpwardSpawnLoc = FVector(0.0, 0.0, 10);
	
	UPROPERTY(EditAnywhere)
	bool ShootUpward = false;
	
	UPROPERTY(VisibleAnywhere, replicated)
	FString SpectatorsStr;

protected:
	/*True means that we're currently in air - or falling*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling;
	
	/*Updates the above properties*/
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimation();
	
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateRotation(const float Value);
};
