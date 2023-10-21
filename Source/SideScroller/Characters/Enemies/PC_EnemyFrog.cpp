// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_EnemyFrog.h"

#include "Components/BoxComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

APC_EnemyFrog::APC_EnemyFrog()
{
	PrimaryActorTick.bCanEverTick = true;
	
	this->GetDamageBox()->SetRelativeScale3D(FVector(-0.100219,0.120000,0.252439));
	
	this->GetLeftHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.179930));
	this->GetLeftHurtBox()->SetRelativeLocation(FVector(-7.000000,0.000000,-1.000000));
	
	this->GetRightHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.179930));
	this->GetRightHurtBox()->SetRelativeLocation(FVector(7.000000,0.000000,-1.000000));

}

void APC_EnemyFrog::BeginPlay()
{
	Super::BeginPlay();
	
	srand(static_cast<unsigned>(time(nullptr)));
	// Get a random number less than JumpPeriod
	const int RandomAmountOfTime = rand() % JumpPeriod;
	
	GetWorld()->GetTimerManager().SetTimer(
		this->JumpTimerHandle,
		this,
		&APC_EnemyFrog::Jump,
		RandomAmountOfTime,
		false
	);
}

void APC_EnemyFrog::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAnimation();
}

void APC_EnemyFrog::SetProjectileTransform(
	const float Direction,
	AActor* MyOwner,
	const ABasePaperCharacter* BaseChar,
	const ABaseProjectile* Projectile
) {
	Projectile->GetProjectileFlipbook()->SetRelativeRotation(BaseChar->GetActorRotation());
	Projectile->GetProjectileMovementComp()->Velocity = FVector(
		Direction * Projectile->GetMovementSpeed(), 0.f, 0.f
	);
}

void APC_EnemyFrog::UpdateAnimation()
{
	// early return if frog enemy is dead
	if (this->IsDead()) {return;}

	if (this->GetMovementComponent()->IsFalling()) {
		if (this->GetVelocity().Z > 0.f) {
			this->GetSprite()->SetFlipbook(JumpAnimation);
		}
		else {
			this->GetSprite()->SetFlipbook(FallAnimation);
		}
	} else {
		this->GetSprite()->SetFlipbook(IdleAnimation);
	}
}

void APC_EnemyFrog::Jump()
{
	Super::Jump();

	this->GetSprite()->SetFlipbook(JumpAnimation);
	
	GetWorld()->GetTimerManager().SetTimer(
		this->JumpTimerHandle,
		this,
		&APC_EnemyFrog::Jump,
		JumpPeriod,
		false
	);

	UGameplayStatics::SpawnSoundAttached(
		this->FrogJumpSound,
		this->GetSprite(),
		TEXT("FrogJumpSound")
	);
}




