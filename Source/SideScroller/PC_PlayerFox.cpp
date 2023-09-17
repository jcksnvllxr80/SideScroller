// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_PlayerFox.h"

#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
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

	this->GetCharacterMovement()->MaxWalkSpeed = 300.0;
	this->GetCharacterMovement()->JumpZVelocity = 600.0;
	this->GetCharacterMovement()->GravityScale = 3.3;
	this->GetCharacterMovement()->AirControl = 0.4;
}

void APC_PlayerFox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APC_PlayerFox::MoveRight);
	PlayerInputComponent->BindAction("ClimbUp", IE_Pressed, this, &APC_PlayerFox::ClimbUp);
	PlayerInputComponent->BindAction("ClimbUp", IE_Released, this, &APC_PlayerFox::StopClimb);
	PlayerInputComponent->BindAction("CrouchClimbDown", IE_Pressed, this, &APC_PlayerFox::CrouchClimbDown);
	PlayerInputComponent->BindAction("CrouchClimbDown", IE_Released, this, &APC_PlayerFox::StopCrouchClimb);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APC_PlayerFox::Jump);
}

void APC_PlayerFox::BeginPlay()
{
	Super::BeginPlay();
}

void APC_PlayerFox::Jump()
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	
	// dont allow another jump unless not currently jumping
	if (!this->bIsFalling) {
		UGameplayStatics::SpawnSoundAttached(
			this->JumpSound,
			this->GetSprite(),
			TEXT("MyPaperCharacterSpriteJump")
		);
		Super::Jump();
	}
}

void APC_PlayerFox::CrouchClimbDown()
{
	if (OverlappingClimbable)
	{
		Climb(-climb_speed);
	} else {
		this->Crouching = true;
	}
}

void APC_PlayerFox::ClimbUp()
{
	if (OverlappingClimbable)
	{
		Climb(climb_speed);
	}
}

void APC_PlayerFox::StopCrouchClimb()
{
	this->Crouching = false;
	this->StopClimb();
}

void APC_PlayerFox::StopClimb()
{
	this->Climbing = false;
}

// Called every frame
void APC_PlayerFox::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAnimation();
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
		MovementSpeed = this->GetVelocity().Size();

		if (bIsFalling) {
			this->GetSprite()->SetFlipbook(JumpAnimation);
		}
		else if (Crouching) {
			this->GetSprite()->SetFlipbook(CrouchAnimation);
		} else if (Climbing) {
			this->GetSprite()->SetFlipbook(ClimbAnimation);
		}
		else if (MovementSpeed != 0.f) {
			this->GetSprite()->SetFlipbook(RunAnimation);
			UGameplayStatics::SpawnSoundAttached(
				this->WalkSound,
				this->GetSprite(),
				TEXT("MyPaperCharacterSpriteWalk")
			);
		}
		else {
			this->GetSprite()->SetFlipbook(IdleAnimation);
		}
	}
}

void APC_PlayerFox::UpdateRotation(const float Value)
{
	if (Value < 0) {
		this->GetSprite()->SetRelativeRotation(FRotator(0, 180.f, 0));
	}
	else {
		this->GetSprite()->SetRelativeRotation(FRotator(0, 0, 0));
	}
}

void APC_PlayerFox::SetOverlappingClimbable(bool bOverlappingClimbable, ABaseClimbable* OverlappedClimbable)
{
	OverlappingClimbable = bOverlappingClimbable;
	NearbyClimbableSound = OverlappedClimbable->LadderSound;
}

void APC_PlayerFox::MoveRight(const float Axis)
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	// early return if player is crouching right now
	if (this->Crouching || this->Climbing ) {return;}
	
	UpdateRotation(Axis);
	AddMovementInput(FVector(Axis, 0, 0));
	if (!OverlappingClimbable && !bIsFalling)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void APC_PlayerFox::Climb(float Direction)
{
	UGameplayStatics::SpawnSoundAttached(
		this->NearbyClimbableSound,
		this->GetSprite(),
		TEXT("ClimbingSound")
	);
	this->Climbing = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	AddMovementInput(GetActorUpVector(), Direction);
}
