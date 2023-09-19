// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePaperCharacter.h"

#include "Enemies/EnemyCollisionPaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"

ABasePaperCharacter::ABasePaperCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Health = DefaultHealth;

	this->GetCharacterMovement()->bConstrainToPlane = true;
	this->GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 1, 0));
}

void ABasePaperCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float ABasePaperCharacter::GetDamage() const
{
	return Damage;
}

void ABasePaperCharacter::DoDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("%s's health depleted!"), *this->GetName());
	this->SetActorEnableCollision(false);
	this->GetSprite()->SetLooping(false);
	this->GetSprite()->SetFlipbook(DeathAnimation);
	GetWorld()->GetTimerManager().SetTimer(
		this->DeathTimerHandle,
		this,
		&ABasePaperCharacter::DestroyActor,
		DeathAnimationTime,
		false
	);

	UGameplayStatics::SpawnSoundAttached(
		this->DeathSound,
		this->GetSprite(),
		TEXT("BasePaperCharacterDeath")
	);
}

void ABasePaperCharacter::DoHurt()
{
	if (this->GetName().Contains("Player"))
	{
		this->GetSprite()->SetFlipbook(HurtAnimation);

		// move character some distance away from the enemy after damage incurred
		this->GetMovementComponent()->AddInputVector(FVector(-1, 0, 0));
		
		GetWorld()->GetTimerManager().SetTimer(
			this->HurtTimerHandle,
			this,
			&ABasePaperCharacter::HurtFinishedCallback,
			HurtAnimationTime,
			false
		);
	}

	UGameplayStatics::SpawnSoundAttached(
		this->PainSound,
		this->GetSprite(),
		TEXT("BasePaperCharacterPain")
	);
}

void ABasePaperCharacter::HurtFinishedCallback()
{
	this->GetSprite()->SetFlipbook(IdleAnimation);
}

float ABasePaperCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser)
{
	this->AddHealth(-DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("%s's health: %f"), *this->GetName(), this->GetHealth());

	if (this->GetHealth() <= 0)
	{
		DoDeath();
	} else {
		DoHurt();
	}
	
	return DamageAmount;
}

void ABasePaperCharacter::DestroyActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->DeathTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(this->HurtTimerHandle);
}

void ABasePaperCharacter::AddHealth(const float HealthValue)
{
	this->SetHealth(FMath::Clamp(this->Health + HealthValue, 0.f, this->DefaultHealth));
}

void ABasePaperCharacter::SetHealth(const float HealthValue)
{
	this->Health = HealthValue;
}

float ABasePaperCharacter::GetHealth() const
{
	return Health;
}
