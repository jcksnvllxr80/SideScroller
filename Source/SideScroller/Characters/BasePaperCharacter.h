// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "SideScroller/Projectiles/BaseProjectile.h"
#include "BasePaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ABasePaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* HurtAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProjectileSpawnLoc;

	ABasePaperCharacter();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	float GetShootDelayTime() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void Shoot();
	
	UFUNCTION(BlueprintCallable)
	float GetDamage() const;

	UFUNCTION(BlueprintCallable)
	void SetDamage(const float Damage);
	
	UFUNCTION(BlueprintCallable)
	USceneComponent* GetProjectileSpawnPoint() const;
	
	UFUNCTION(BlueprintCallable)
	void DoDeath();

	UFUNCTION(BlueprintCallable)
	void PushHurtCharacter(AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void DoHurt(AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	void PrepProjectileLaunch(bool bIsPLayer);

	bool PlayerCanShoot();

	bool EnemyCanShoot();

	UFUNCTION(BlueprintCallable)
	void SetHealth(float Health);

	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthValue);

	UFUNCTION(BlueprintPure, Category = "HUD")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "HUD")
	float GetDefaultHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();

	UFUNCTION(BlueprintCallable)
	virtual void HurtFinishedCallback();

	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere)
	USoundBase* PainSound;

private:
	// Properties
	UPROPERTY(EditAnywhere)
	float Health = 0;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere)
	float DeathAnimationTime = 1.0;

	UPROPERTY(EditAnywhere)
	float HurtAnimationTime = 0.5;

	FTimerHandle DeathTimerHandle;
	
	FTimerHandle HurtTimerHandle;

	UPROPERTY(EditAnywhere)
	float DefaultHealth = 100.0;
	
	UPROPERTY(EditAnywhere)
	float Damage = 10.0;
	
	UPROPERTY(EditAnywhere)
	float ShootDelayTime = 2.0;

	UPROPERTY(EditAnywhere)
	float HurtPushAmount = 20.f;

	bool IsDead = false;
};
