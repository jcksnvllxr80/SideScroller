#pragma once
#include "CoreMinimal.h"
#include "../../Climbables/BaseClimbable.h"
#include "../BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "SideScroller/Pickups/Gem.h"
#include "PC_PlayerFox.generated.h"

UCLASS()
class SIDESCROLLER_API APC_PlayerFox : public ABasePaperCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetCherryCount() const;
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetCherryStash(int CherryStash);

	UFUNCTION(BlueprintPure, Category = "HUD")
	int GetMoneyCount() const;
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetMoneyStash(const int MoneyAmount);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CrouchProjectileSpawnPoint = FVector(0.f, -25.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* CrouchAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* ClimbAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* StopOnLadderAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClimbSpeed = 0.17f;

	UFUNCTION(BlueprintCallable)
	void TakeMoney(int MonetaryValue);

	UFUNCTION(BlueprintCallable)
	void TakeHealing(float HealingValue);

	UFUNCTION(BlueprintCallable)
	void TakeCherries(int NumCherries);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetHurtPushAmount() const;
	
	APC_PlayerFox();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Jump() override;

	void CrouchClimbDown();

	void StopCrouchClimb();

	void ClimbUp();

	void StopClimb();

	void SetOverlappingClimbable(bool bOverlappingClimbable, ABaseClimbable* OverlappedClimbable);

private:
	UPROPERTY(EditAnywhere)
	int CherryStash = 0;

	UPROPERTY(EditAnywhere)
	int MoneyStash = 0;
	
	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere)
	USoundBase* WalkSound;

	UPROPERTY(EditAnywhere)
	bool Crouching;

	UPROPERTY(EditAnywhere)
	bool Climbing;

	UPROPERTY(EditAnywhere)
	bool OverlappingClimbable;

	UPROPERTY(EditAnywhere)
	float HurtPushAmount = 20.f;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	USoundBase* NearbyClimbableSound;

	bool OnLadder = false;

	float CumulativeTime = 0.f;

	UFUNCTION(BlueprintCallable)
	void ClimbUpAxisInputCallback(float X);
	
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Axis);

	UFUNCTION(BlueprintCallable)
	void Climb(float Value);

	UFUNCTION(BlueprintCallable)
	void LogSpeed();

	UFUNCTION(BlueprintCallable)
	void LogRotation();

	UFUNCTION(BlueprintCallable)
	void LogLocation();

	UFUNCTION(BlueprintCallable)
	void DoWalkAnimAndSound();

	UFUNCTION(BlueprintCallable)
	void DoClimbAnimAndSound();

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
