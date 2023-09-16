
#pragma once
#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "PaperFlipbook.h"
#include "PC_PayerFox.generated.h"

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
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APC_PlayerFox();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Jump() override;

	void CrouchClimbDown();

	void StopCrouchClimb();

	void ClimbUp();

	void StopClimbUp();

private:
	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere)
	USoundBase* WalkSound;

	UPROPERTY(EditAnywhere)
	bool Crouching;

	UPROPERTY(EditAnywhere)
	bool Climbing;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UFUNCTION(BlueprintCallable)
	void MoveRight(float Axis);

protected:
	/*True means that we're currently in air - or falling*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling;

	/*Holds the current speed of our character*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;


	/*Updates the above properties*/
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimation();
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateRotation(const float Value);
};
