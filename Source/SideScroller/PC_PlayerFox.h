
#pragma once
#include "CoreMinimal.h"
#include "BaseClimbable.h"
#include "BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "PC_PlayerFox.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API APC_PlayerFox : public ABasePaperCharacter
{
	GENERATED_BODY()

public:
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
	float ClimbSpeed = 1.f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere)
	USoundBase* WalkSound;

	UPROPERTY(EditAnywhere)
	bool Crouching;

	UPROPERTY(EditAnywhere)
	bool Climbing;

	UPROPERTY(EditAnywhere)
	bool OverlappingClimbable;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	USoundBase* NearbyClimbableSound;

	bool OnLadder = false;

	UFUNCTION(BlueprintCallable)
	void MoveRight(float Axis);

	UFUNCTION(BlueprintCallable)
	void Climb(float Value);

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