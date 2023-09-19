// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
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

	ABasePaperCharacter();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	float GetDamage() const;

	UFUNCTION(BlueprintCallable)
	void DoDeath();

	UFUNCTION(BlueprintCallable)
	void DoHurt(AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float Health);
	
	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthValue);
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

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
	float Health = 0;

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
	
};
