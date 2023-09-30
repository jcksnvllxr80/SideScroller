#include "PC_PlayerFox.h"

#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

APC_PlayerFox::APC_PlayerFox()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetWorldLocationAndRotation(
		FVector(0.0f, 0.0f, 50.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);
	CameraArm->bDoCollisionTest = false;
	CameraArm->TargetArmLength = 200.f;
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 5.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm);

	this->GetCharacterMovement()->MaxWalkSpeed = 230.0;
	this->GetCharacterMovement()->JumpZVelocity = 525.0;
	this->GetCharacterMovement()->GravityScale = 3.5;
	this->GetCharacterMovement()->AirControl = 0.4;
	this->GetCharacterMovement()->MaxStepHeight = 10.0;
}

void APC_PlayerFox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APC_PlayerFox::MoveRight);
	PlayerInputComponent->BindAxis("ClimbUp", this, &APC_PlayerFox::ClimbUpAxisInputCallback);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APC_PlayerFox::Jump);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APC_PlayerFox::Shoot);
}

void APC_PlayerFox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APC_PlayerFox::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CumulativeTime += DeltaTime;
	// if (CumulativeTime > 0.5f)
	// {
	// 	LogSpeed();
	// 	LogRotation();
	// 	LogLocation();
	// 	CumulativeTime = 0.f;
	// }
	
	UpdateAnimation();
}

int APC_PlayerFox::GetCherryCount() const
{
	return this->CherryStash;
}

void APC_PlayerFox::SetCherryStash(int NumCherries)
{
	this->CherryStash = NumCherries;
}

int APC_PlayerFox::GetMoneyCount() const
{
	return this->MoneyStash;
}

void APC_PlayerFox::SetMoneyStash(const int MoneyAmount)
{
	this->MoneyStash = MoneyAmount;
}

void APC_PlayerFox::DoWalkAnimAndSound()
{
	if (this->GetSprite()->GetFlipbook() != RunAnimation) {
		this->GetSprite()->SetFlipbook(RunAnimation);
	}
			
	if (this->GetSprite()->GetPlaybackPositionInFrames() % 12 == 0) {
		// UE_LOG(LogTemp, Warning, TEXT("Playing %s's walking sound!"), *this->GetName());
		UGameplayStatics::SpawnSoundAttached(
			this->WalkSound,
			this->GetSprite(),
			TEXT("PaperCharacterSpriteWalk")
		);
	}
}

void APC_PlayerFox::DoClimbAnimAndSound()
{
	if (this->GetVelocity().Z == 0) {
		if (this->GetSprite()->GetFlipbook() != StopOnLadderAnimation) {
			this->GetSprite()->SetFlipbook(StopOnLadderAnimation);
		}
	} else {
		if (this->GetSprite()->GetFlipbook() != ClimbAnimation) this->GetSprite()->SetFlipbook(ClimbAnimation);
		if (this->GetSprite()->GetPlaybackPositionInFrames() == 0) {
			// UE_LOG(LogTemp, Warning, TEXT("Playing %s's climbing sound!"), *this->GetName());
			UGameplayStatics::SpawnSoundAttached(
				this->NearbyClimbableSound,
				this->GetSprite(),
				TEXT("ClimbingSound")
			);
		}
	}
}

void APC_PlayerFox::UpdateAnimation()
{
	if (this)
	{
		// early return if player in hurt animation right now
		if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
		
		//Update our falling property
		bIsFalling = this->GetMovementComponent()->IsFalling();
 
		//Update our movement speed
		float MovementSpeed = this->GetVelocity().Size();

		if (bIsFalling) {
			this->GetSprite()->SetFlipbook(JumpAnimation);
		}
		else if (this->Crouching) {
			this->GetSprite()->SetFlipbook(CrouchAnimation);
		} else if (this->Climbing || this->OnLadder) {
			DoClimbAnimAndSound();
		}
		else if (MovementSpeed != 0.f) {
			DoWalkAnimAndSound();
		}
		else {
			this->GetSprite()->SetFlipbook(IdleAnimation);
		}
	}
}

void APC_PlayerFox::UpdateRotation(const float Value)
{
	const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
	
	if (Value < 0) {
		this->GetSprite()->SetRelativeRotation(FRotator(0, 180.f, 0));
		GetProjectileSpawnPoint()->SetRelativeLocation(FVector(ProjectileSpawnLoc.X * -1.f, ProjSpawnLoc.Y, ProjSpawnLoc.Z));
	}
	else {
		this->GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
		GetProjectileSpawnPoint()->SetRelativeLocation(FVector(ProjectileSpawnLoc.X, ProjSpawnLoc.Y, ProjSpawnLoc.Z));
	}
}

void APC_PlayerFox::SetOverlappingClimbable(bool bOverlappingClimbable, ABaseClimbable* OverlappedClimbable)
{
	OverlappingClimbable = bOverlappingClimbable;
	NearbyClimbableSound = OverlappedClimbable->LadderSound;
	if (!OverlappingClimbable)
	{
		this->OnLadder = false;
		this->Climbing = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void APC_PlayerFox::CrouchClimbDown()
{
	if (OverlappingClimbable)
	{
		Climb(-ClimbSpeed);
	} else {
		this->Crouching = true;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(ProjectileSpawnLoc - CrouchProjectileSpawnPoint);
	}
}

void APC_PlayerFox::ClimbUp()
{
	if (OverlappingClimbable)
	{
		this->OnLadder = true;
		Climb(ClimbSpeed);
	}
}

void APC_PlayerFox::StopCrouchClimb()
{
	if (this->Crouching)
	{
		const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
		this->Crouching = false;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(
			FVector(ProjSpawnLoc.X, ProjectileSpawnLoc.Y, ProjSpawnLoc.Z)
		);
	}
	this->StopClimb();
}

void APC_PlayerFox::StopClimb()
{
	if (this->OnLadder && this->GetVelocity().Z != 0)
	{
		this->GetMovementComponent()->StopMovementImmediately();
	}
	this->Climbing = false;
}

void APC_PlayerFox::ClimbUpAxisInputCallback(const float X)
{
	if (X > 0) {
		ClimbUp();
	} else if (X < 0) {
		CrouchClimbDown();
	} else {
		StopCrouchClimb();
	}
}

float APC_PlayerFox::GetHurtPushAmount() const
{
	return HurtPushAmount;
}

void APC_PlayerFox::MoveRight(const float Axis)
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	// early return if player is crouching right now
	if (this->Crouching || this->Climbing) {return;}
	
	UpdateRotation(Axis);
	AddMovementInput(FVector(Axis, 0, 0));
	if (!OverlappingClimbable && !bIsFalling)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void APC_PlayerFox::Climb(const float Value)
{
	this->Climbing = true;
	this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	AddMovementInput(GetActorUpVector(), Value);
}

void APC_PlayerFox::Jump()
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	
	// dont allow another jump unless not currently jumping
	if (!this->bIsFalling && !this->OnLadder) {
		UGameplayStatics::SpawnSoundAttached(
			this->JumpSound,
			this->GetSprite(),
			TEXT("PaperCharacterSpriteJump")
		);
		Super::Jump();
	}
}

void APC_PlayerFox::TakeMoney(int MonetaryValue)
{
	this->MoneyStash += MonetaryValue;
	UE_LOG(LogTemp, Warning, TEXT("%s's money stash is now %i!"), *this->GetName(), this->MoneyStash);

}

void APC_PlayerFox::TakeHealing(const float HealingValue)
{
	this->AddHealth(HealingValue);
	UE_LOG(LogTemp, Warning, TEXT("%s's health is now %f!"), *this->GetName(), this->GetHealth());
}

void APC_PlayerFox::TakeCherries(int NumCherries)
{
	this->CherryStash += NumCherries;
	UE_LOG(LogTemp, Warning, TEXT("%s's cherry stash has increased to %d!"), *this->GetName(), this->CherryStash);
}

void APC_PlayerFox::LogSpeed()
{
	UE_LOG(LogTemp, Warning, TEXT("%s's speed is %f!"), *this->GetName(), this->GetVelocity().Size());
}

void APC_PlayerFox::LogRotation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s's rotation is %s!"), *this->GetName(), *GetSprite()->GetRelativeRotation().ToString());
}

void APC_PlayerFox::LogLocation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s's location is %s!"), *this->GetName(), *this->GetActorLocation().ToString());
}
