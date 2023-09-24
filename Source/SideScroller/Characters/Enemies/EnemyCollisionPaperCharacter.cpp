// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCollisionPaperCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

AEnemyCollisionPaperCharacter::AEnemyCollisionPaperCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	this->DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	this->DamageBox->SetupAttachment(RootComponent);
	this->DamageBox->SetHiddenInGame(true);

	this->LeftHurtBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHurtBox"));
	this->LeftHurtBox->SetupAttachment(RootComponent);
	this->LeftHurtBox->SetHiddenInGame(true);

	this->RightHurtBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHurtBox"));
	this->RightHurtBox->SetupAttachment(RootComponent);
	this->RightHurtBox->SetHiddenInGame(true);
}

void AEnemyCollisionPaperCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->DamageBox->SetNotifyRigidBodyCollision(true);
	this->DamageBox->OnComponentHit.AddDynamic(this, &AEnemyCollisionPaperCharacter::OnHitDelegate);

	this->LeftHurtBox->SetGenerateOverlapEvents(true);
	this->LeftHurtBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCollisionPaperCharacter::OnBeginOverlapDelegate);

	this->RightHurtBox->SetGenerateOverlapEvents(true);
	this->RightHurtBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCollisionPaperCharacter::OnBeginOverlapDelegate);
}

UBoxComponent* AEnemyCollisionPaperCharacter::GetDamageBox() const
{
	return DamageBox;
}

UBoxComponent* AEnemyCollisionPaperCharacter::GetLeftHurtBox() const
{
	return LeftHurtBox;
}

UBoxComponent* AEnemyCollisionPaperCharacter::GetRightHurtBox() const
{
	return RightHurtBox;
}

void AEnemyCollisionPaperCharacter::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult
){
	ABasePaperCharacter* OverlappingActor = dynamic_cast<ABasePaperCharacter*>(OtherComp->GetOwner());
	if (OverlappingActor == nullptr) return;
	
	if (OverlappingActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has overlapped %s!"),
		       *OtherActor->GetName(),
		       *OverlappedComponent->GetOwner()->GetName()
		);
		OverlappingActor->TakeDamage(
			this->GetDamage(),
			FDamageEvent(UDamageType::StaticClass()),
			this->GetInstigatorController(),
			this
		);
	}
}

void AEnemyCollisionPaperCharacter::OnHitDelegate(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
) {
	UE_LOG(LogTemp, Warning, TEXT("%s has hit %s!"),
		*OtherActor->GetName(),
		*HitComponent->GetOwner()->GetName()
	);
	ABasePaperCharacter* DamagingActor = dynamic_cast<ABasePaperCharacter*>(OtherComp->GetOwner());
	if (DamagingActor == nullptr) return;
	
	this->TakeDamage(
		DamagingActor->GetDamage(),
		FDamageEvent(UDamageType::StaticClass()),
		this->GetInstigatorController(),
		this
	);
}
