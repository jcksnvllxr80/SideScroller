#pragma once
#include "CoreMinimal.h"
#include "SideScroller/Climbables/BaseClimbable.h"
#include "SideScroller/Characters/BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"
#include "PC_PlayerFox.generated.h"


class ASideScrollerGameModeBase;
class USideScrollerGameInstance;

UCLASS()
class SIDESCROLLER_API APC_PlayerFox : public ABasePaperCharacter, public IProjectileInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UPrimitiveComponent* GetInteractableObject() const;

	UFUNCTION(BlueprintCallable)
	void SetInteractableObject(UPrimitiveComponent* InteractableObj);

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

	UFUNCTION(BlueprintCallable)
	void ClearInteractableObject();

	UFUNCTION(BlueprintCallable)
	void DoLevelComplete();

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

	UPROPERTY(EditAnywhere, Category = Camera)
	UTextRenderComponent* NameBanner;
	
	FTimerHandle ShootTimerHandle;

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

	UFUNCTION(BlueprintCallable)
	void UseAction();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void LoadProfilePlayerName();
	
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

	UFUNCTION(BlueprintCallable)
	void DisplayGameMessage(FText Message);

	UFUNCTION(BlueprintCallable)
	void HideGameMessage() const;

private:
	UPROPERTY()
	bool bPlayerNameSentToServer = false;

	UPROPERTY()
	bool bPlayerNameSet = false;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SendPlayerNameToServer(const FString& ClientPlayerName);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SendRotationToServer(const float Value);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SendVerticalAxisInputToServer(const float Z);
	
	UPROPERTY()
	bool bCanShoot = true;

	UPROPERTY()
	FRotator LastRotation;
	
	UPROPERTY()
	UPrimitiveComponent* InteractableObject;

	UPROPERTY()
	USideScrollerGameInstance* GameInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetPlayerHUD;

	UPROPERTY(EditAnywhere)
	int32 FramesPerStep = 12;
	
	UPROPERTY(EditAnywhere, replicated)
	UUserWidget* WidgetPlayerHUDInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetPlayerGameMessage;
	
	UPROPERTY()
	UUserWidget* WidgetPlayerGameMessageInstance;
	
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

	UPROPERTY(EditAnywhere)
	float MaxRunningSpeed = 500.f;
	
	UPROPERTY(EditAnywhere)
	double CrouchSlidingThresholdVelocity = 200.f;

	UPROPERTY(EditAnywhere)
	int CherryStash = 0;

	UPROPERTY(EditAnywhere)
	int MoneyStash = 0;
	
	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, replicated)
	USoundBase* WalkSound;

	UPROPERTY(EditAnywhere)
	USoundBase* LevelStartSound;

	UPROPERTY(EditAnywhere)
	USoundBase* LevelCompleteSound;

	UPROPERTY(EditAnywhere, replicated)
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, replicated)
	bool bIsClimbing = false;

	UPROPERTY(EditAnywhere, replicated)
	bool bIsSliding = false;

	UPROPERTY(EditAnywhere, replicated)
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

	UPROPERTY()
	USoundBase* NearbyClimbableSound;

	UPROPERTY(EditAnywhere)
	float CumulativeTime = 0.f;

	UPROPERTY(EditAnywhere, replicated)
	FRotator CurrentRotation;

	UPROPERTY(EditAnywhere)
	FRotator MovingLeftRotation = FRotator(0, 180.f, 0);

	UPROPERTY(EditAnywhere)
	FRotator MovingRightRotation = FRotator(0, 0, 0);

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


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetRunVelocityRPC();

	UFUNCTION(BlueprintCallable)
	void SetRunVelocity();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetWalkVelocityRPC();

	UFUNCTION(BlueprintCallable)
	void SetWalkVelocity();
	
	UFUNCTION(BlueprintCallable)
	void LogLocation();

	UFUNCTION(BlueprintCallable)
	void OpenMenu();
	
	UFUNCTION(BlueprintCallable)
	void OpenInGameMenu();

	UFUNCTION()
	void CanShootAgain();
	
	virtual void Shoot() override;

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

	UFUNCTION(BlueprintCallable)
	void PlayerMessageWidgetTeardown();

	UFUNCTION(BlueprintCallable)
	void PlayerGameMessageSetup();

	UFUNCTION(BlueprintCallable)
	void GetMessageWidgetTextBlock(UTextBlock*& TextBlock) const;

	UFUNCTION()
	void DoLevelWelcome();
	
	UPROPERTY(EditAnywhere)
	FVector CrouchProjectileSpawnPoint = FVector(0.f, 0.f, 5.f);

	UPROPERTY(EditAnywhere)
	FVector ProjectileUpwardSpawnLoc = FVector(0.0, 0.0, 10);
	
	UPROPERTY(EditAnywhere)
	bool ShootUpward = false;
	
	UPROPERTY(VisibleAnywhere, replicated)
	FString SpectatorsStr;

	UPROPERTY(EditAnywhere, Replicated)
	FString PlayerName;

	FTimerHandle LevelStartMessageTimerHandle;

	FTimerHandle LevelCompleteMessageTimerHandle;

	FTimerHandle LevelWelcomeDelayTimerHandle;

	UPROPERTY(EditAnywhere)
	float LevelWelcomeDelayTime = 0.5;
	
	UPROPERTY(EditAnywhere)
	float LevelStartMessageTime = 3.f;

	UPROPERTY(EditAnywhere)
	float LevelCompleteMessageTime = 3.f;
	
protected:
	/*True means that we're currently in air - or falling*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling;
	
	/*Updates the above properties*/
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimation();
	
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateRotation(const float Value);
	
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateNameBanner();
};
