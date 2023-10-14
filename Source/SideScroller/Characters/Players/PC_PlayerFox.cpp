#include "PC_PlayerFox.h"

#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/SideScrollerGameModeBase.h"

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

	this->GetCharacterMovement()->MaxWalkSpeed = 250.0;
	this->GetCharacterMovement()->JumpZVelocity = 525.0;
	this->GetCharacterMovement()->GravityScale = 3.5;
	this->GetCharacterMovement()->AirControl = 0.4;
	this->GetCharacterMovement()->MaxStepHeight = 10.0;
	this->GetCharacterMovement()->SetWalkableFloorAngle(60.f);
	this->GetCharacterMovement()->MaxAcceleration = 400.f;
	this->GetCharacterMovement()->BrakingFrictionFactor = 0.65;
}

void APC_PlayerFox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APC_PlayerFox::MoveRight);
	PlayerInputComponent->BindAxis("ClimbUp", this, &APC_PlayerFox::ClimbUpAxisInputCallback);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APC_PlayerFox::Jump);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APC_PlayerFox::Shoot);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APC_PlayerFox::SetRunVelocity);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APC_PlayerFox::SetWalkVelocity);
	PlayerInputComponent->BindAction("InGameMenu", IE_Pressed, this, &APC_PlayerFox::OpenInGameMenu);

}

void APC_PlayerFox::BeginPlay()
{
	Super::BeginPlay();

	AddToPlayersArray(this);
	PlayerHUDSetup();
	
	this->LastCheckpointLocation = this->GetSprite()->GetComponentLocation(); 
	this->StandingFriction = this->GetCharacterMovement()->BrakingFrictionFactor;
	this->NormalWalkingSpeed = this->GetCharacterMovement()->MaxWalkSpeed;
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

int APC_PlayerFox::GetAccumulatedPoints() const
{
	return this->AccumulatedPoints;
}

void APC_PlayerFox::SetAccumulatedPoints(const int Points)
{
	this->AccumulatedPoints = Points;
}

int APC_PlayerFox::GetNumberOfLives() const
{
	return this->NumberOfLives;
}

void APC_PlayerFox::SetNumberOfLives(const int NumLives)
{
	this->NumberOfLives = NumLives;
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

void APC_PlayerFox::SetCheckpointLocation(const FVector& Location)
{
	this->LastCheckpointLocation = Location;
}

void APC_PlayerFox::ReviveAtCheckpoint()
{
	// set location back to last checkpoint
	this->SetHealth(this->DefaultHealth);
	this->SetActorLocation(
		LastCheckpointLocation, false, nullptr, ETeleportType::ResetPhysics
	);
}

void APC_PlayerFox::PlayerDeath()
{
	if (this->NumberOfLives > 0)
	{
		// take a life away
		this->NumberOfLives -= 1;
		
		ReviveAtCheckpoint();
	} else {
		RemoveFromPlayersArray(this);
		// detach HUD
		this->WidgetPlayerHUDInstance->RemoveFromParent();
		this->DoDeath();
		// TODO: get another player to spectate
	}
}

void APC_PlayerFox::HandleFallOffLevel()
{
	// Maybe one color character doesnt die when it falls off, wrap PlayerDeath in that case.
	PlayerDeath();
}

void APC_PlayerFox::DoWalkAnimAndSound()
{
	if (this->GetSprite()->GetFlipbook() != RunAnimation) {
		this->GetSprite()->SetFlipbook(RunAnimation);
	}
			
	if (this->GetSprite()->GetPlaybackPositionInFrames() % 12 == 0) {
		// UE_LOG(LogTemp, VeryVerbose, TEXT("Playing %s's walking sound!"), *this->GetName());
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
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Playing %s's climbing sound!"), *this->GetName());
			UGameplayStatics::SpawnSoundAttached(
				this->NearbyClimbableSound,
				this->GetSprite(),
				TEXT("ClimbingSound")
			);
		}
	}
}

void APC_PlayerFox::AddToPlayersArray(APC_PlayerFox* Apc_PlayerFox)
{
	if (ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode())
	) {
		GameMode->AddPlayer(this);	
	}
}

void APC_PlayerFox::RemoveFromPlayersArray(APC_PlayerFox* Apc_PlayerFox)
{
	if (ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode())
	) {
		GameMode->RemovePlayer(this);	
	}
}

void APC_PlayerFox::PlayerHUDSetup()
{
	WidgetPlayerHUDInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetPlayerHUD);
	WidgetPlayerHUDInstance->AddToViewport();
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
		const float MovementSpeed = this->GetVelocity().Size();

		if (bIsFalling) {
			if (this->GetVelocity().Z > 0.f) {
				this->GetSprite()->SetFlipbook(JumpAnimation);
			}
			else {
				this->GetSprite()->SetFlipbook(FallAnimation);
			}
		}
		else if (this->bIsCrouching) {
			this->GetSprite()->SetFlipbook(CrouchAnimation);
		} else if (this->bIsClimbing || this->bOnLadder) {
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
	else if (Value > 0){
		this->GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
		GetProjectileSpawnPoint()->SetRelativeLocation(FVector(ProjectileSpawnLoc.X, ProjSpawnLoc.Y, ProjSpawnLoc.Z));
	}
	
	// UE_LOG(
	// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
	// 	*this->GetName(),
	// 	*this->GetActorLocation().ToString(),
	// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
	// );
}

void APC_PlayerFox::SetOverlappingClimbable(bool OverlappingClimbable, ABaseClimbable* OverlappedClimbable)
{
	bOverlappingClimbable = OverlappingClimbable;
	NearbyClimbableSound = OverlappedClimbable->LadderSound;
	if (!bOverlappingClimbable)
	{
		this->bOnLadder = false;
		this->bIsClimbing = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void APC_PlayerFox::SetProjectileTransform(
	const float Direction,
	AActor* MyOwner,
	const ABasePaperCharacter* BaseChar,
	const ABaseProjectile* Projectile
) {
	if (ShootUpward)
	{
		Projectile->GetProjectileFlipbook()->SetRelativeRotation(FRotator( 90, 0, 0));
		Projectile->GetProjectileMovementComp()->Velocity = FVector(
			0.f, 0.f, Projectile->GetMovementSpeed()
		);
	}
	else
	{
		Projectile->GetProjectileFlipbook()->SetRelativeRotation(BaseChar->GetSprite()->GetRelativeRotation());
		Projectile->GetProjectileMovementComp()->Velocity = FVector(
			Direction * Projectile->GetMovementSpeed(), 0.f, 0.f
		);
	}
}

void APC_PlayerFox::CrouchClimbDown()
{
	if (!this->bIsSliding && abs(this->GetVelocity().X) > this->CrouchSlidingThresholdVelocity)
	{
		this->bIsSliding = true;
		this->GetCharacterMovement()->BrakingFrictionFactor = this->CrouchSlideFriction;
		return;
	}
	
	if (bOverlappingClimbable && !this->bIsSliding)
	{
		Climb(-ClimbSpeed);
	} else {
		this->bIsCrouching = true;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(
			FVector(
				this->GetProjectileSpawnPoint()->GetRelativeLocation().X,
				0.f,
				ProjectileSpawnLoc.Z - CrouchProjectileSpawnPoint.Z
			)
		);
		
		// UE_LOG(
		// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
		// 	*this->GetName(),
		// 	*this->GetActorLocation().ToString(),
		// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
		// );
	}
}

void APC_PlayerFox::ClimbUp()
{
	if (bOverlappingClimbable)
	{
		this->bOnLadder = true;
		Climb(ClimbSpeed);
	}
	else
	{
		// const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
		this->bIsCrouching = false;
		this->ShootUpward = true;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(ProjectileUpwardSpawnLoc);
		
		// UE_LOG(
		// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
		// 	*this->GetName(),
		// 	*this->GetActorLocation().ToString(),
		// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
		// );
	}
}

void APC_PlayerFox::StopCrouchClimb()
{
	if (this->bIsCrouching)
	{
		const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
		this->bIsSliding = false;
		this->GetCharacterMovement()->BrakingFrictionFactor = this->StandingFriction;
		this->bIsCrouching = false;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(
			FVector(ProjSpawnLoc.X, ProjectileSpawnLoc.Y, ProjSpawnLoc.Z)
		);
		
		// UE_LOG(
		// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
		// 	*this->GetName(),
		// 	*this->GetActorLocation().ToString(),
		// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
		// );
	}

	// set ShootUpward to false if its true
	this->ShootUpward &= false;
	
	const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
	this->GetProjectileSpawnPoint()->SetRelativeLocation(
		FVector(ProjSpawnLoc.X, ProjectileSpawnLoc.Y, ProjectileSpawnLoc.Z)
	);
	
	// UE_LOG(
	// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
	// 	*this->GetName(),
	// 	*this->GetActorLocation().ToString(),
	// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
	// );
	
	this->StopClimb();
}

void APC_PlayerFox::StopClimb()
{
	if (this->bOnLadder && this->GetVelocity().Z != 0)
	{
		this->GetMovementComponent()->StopMovementImmediately();
	}
	this->bIsClimbing = false;
}

void APC_PlayerFox::ClimbUpAxisInputCallback(const float Z)
{
	if (Z > 0) {
		ClimbUp();
	} else if (Z < 0) {
		CrouchClimbDown();
	} else {
		StopCrouchClimb();
	}
}

void APC_PlayerFox::MoveRight(const float Axis)
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	// early return if player is crouching right now
	if (this->bIsCrouching || this->bIsClimbing) {return;}
	
	UpdateRotation(Axis);

	// on ladder == flying
	if (GetCharacterMovement()->GetMovementName() == "Flying")
	{
		if (Axis == 0)  // if not pressing move left or right button
		{
			GetMovementComponent()->StopMovementImmediately();
		}
		else
		{
			// move horizontally on ladder at a reduced speed compared to normal
			AddMovementInput(FVector(Axis * ClimbingLateralSpeed, 0, 0));
		}
	}
	else
	{
		// move left/right normally
		AddMovementInput(FVector(Axis, 0, 0));
	}


	if (!bOverlappingClimbable && !bIsFalling)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void APC_PlayerFox::Climb(const float Value)
{
	if (!this->bIsClimbing){
		GetMovementComponent()->StopMovementImmediately();
		this->bIsClimbing = true;
	}
	this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	AddMovementInput(GetActorUpVector(), Value);
}

void APC_PlayerFox::Jump()
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	
	// dont allow another jump unless not currently jumping
	if (!this->bIsFalling && !this->bOnLadder) {
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
	UE_LOG(LogTemp, Verbose, TEXT("%s's money stash is now %i!"), *this->GetName(), this->MoneyStash);
}

void APC_PlayerFox::TakeHealing(const float HealingValue)
{
	this->AddHealth(HealingValue);
	UE_LOG(LogTemp, Verbose, TEXT("%s's health is now %f!"), *this->GetName(), this->GetHealth());
}

void APC_PlayerFox::TakeCherries(int NumCherries)
{
	this->CherryStash += NumCherries;
	UE_LOG(LogTemp, Verbose, TEXT("%s's cherry stash has increased to %d!"), *this->GetName(), this->CherryStash);
}

void APC_PlayerFox::LogSpeed()
{
	UE_LOG(LogTemp, VeryVerbose, TEXT("%s's speed is %f!"), *this->GetName(), this->GetVelocity().Size());
}

void APC_PlayerFox::LogRotation()
{
	UE_LOG(LogTemp, VeryVerbose, TEXT("%s's rotation is %s!"), *this->GetName(), *GetSprite()->GetRelativeRotation().ToString());
}

void APC_PlayerFox::SetRunVelocity()
{
	this->GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
}

void APC_PlayerFox::SetWalkVelocity()
{
	this->GetCharacterMovement()->MaxWalkSpeed = NormalWalkingSpeed;
}

void APC_PlayerFox::LogLocation()
{
	UE_LOG(LogTemp, VeryVerbose, TEXT("%s's location is %s!"), *this->GetName(), *this->GetActorLocation().ToString());
}

void APC_PlayerFox::OpenInGameMenu()
{
	GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance != nullptr) {
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GameInstance->InGameLoadMenu();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't open InGameMenu!"));
	}
}
