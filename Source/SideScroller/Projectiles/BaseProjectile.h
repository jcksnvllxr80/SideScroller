// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

class ABasePaperCharacter;

UCLASS()
class SIDESCROLLER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectile();

	UFUNCTION(BlueprintCallable)
	float GetMovementSpeed() const;
	
	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(const float XDirection);

	UFUNCTION(BlueprintCallable)
	void DoCollisionAnimAndSound(const AActor* MyOwner, ABasePaperCharacter* OtherBasePaperActor);

	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovementComp() const;

	UFUNCTION(BlueprintCallable)
	UPaperFlipbookComponent* GetProjectileFlipbook() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UPaperFlipbook* TravelAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UPaperFlipbook* CollisionAnimation;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* LaunchSound;
	
	UPROPERTY(VisibleAnywhere, Category = Actor)
	UPaperFlipbookComponent* ProjectileFlipbook;

	UPROPERTY(EditAnywhere)
	float CollisionAnimationTime = 1.0;
	
	FTimerHandle CollisionTimerHandle;
	
	UPROPERTY(VisibleAnywhere, Category = Collision)
	UCapsuleComponent* ProjectileBox;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	UProjectileMovementComponent* ProjectileMovementComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lifespan", meta = (AllowPrivateAccess = "true"))
	float ProjectileInLifespan = 0.5f;

	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
