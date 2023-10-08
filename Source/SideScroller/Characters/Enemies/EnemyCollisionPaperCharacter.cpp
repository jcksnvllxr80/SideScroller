// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCollisionPaperCharacter.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

AEnemyCollisionPaperCharacter::AEnemyCollisionPaperCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
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
	this->DamageBox->SetCollisionProfileName("BlockAllDynamic");

	this->LeftHurtBox->SetGenerateOverlapEvents(true);
	this->LeftHurtBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCollisionPaperCharacter::OnBeginOverlapDelegate);
	this->LeftHurtBox->SetCollisionProfileName("OverlapAllDynamic");

	this->RightHurtBox->SetGenerateOverlapEvents(true);
	this->RightHurtBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCollisionPaperCharacter::OnBeginOverlapDelegate);
	this->RightHurtBox->SetCollisionProfileName("OverlapAllDynamic");
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

int AEnemyCollisionPaperCharacter::GetPointWorth() const
{
	return this->PointWorth;
}

void AEnemyCollisionPaperCharacter::GivePoints(APC_PlayerFox* PlayerChar)
{
	if (PlayerChar == nullptr) return;

	UE_LOG(LogTemp, Display, TEXT("%s giving %s %d points!"),
	   *this->GetName(), *PlayerChar->GetName(), this->PointWorth
	);
	PlayerChar->SetAccumulatedPoints(PlayerChar->GetAccumulatedPoints() + this->PointWorth);
}

void AEnemyCollisionPaperCharacter::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult
){
	APC_PlayerFox* OverlappingActor = dynamic_cast<APC_PlayerFox*>(OtherComp->GetOwner());
	if (OverlappingActor == nullptr) return;
	

	UE_LOG(LogTemp, Verbose, TEXT("%s has overlapped %s!"),
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

void AEnemyCollisionPaperCharacter::OnHitDelegate(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
) {
	UE_LOG(LogTemp, Display, TEXT("%s has hit %s!"),
		*OtherActor->GetName(),
		*HitComponent->GetOwner()->GetName()
	);
	const APC_PlayerFox* DamagingActor = dynamic_cast<APC_PlayerFox*>(OtherComp->GetOwner());
	if (DamagingActor == nullptr) return;
	
	this->TakeDamage(
		DamagingActor->GetDamage(),
		FDamageEvent(UDamageType::StaticClass()),
		this->GetInstigatorController(),
		OtherActor
	);
}
