// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScroller/Characters/BasePaperCharacter.h"
#include "SideScroller/Interfaces/PointsInterface.h"
#include "EnemyCollisionPaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER_API AEnemyCollisionPaperCharacter : public ABasePaperCharacter, public IPointsInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	AEnemyCollisionPaperCharacter();

	UFUNCTION(BlueprintCallable)
	void OnHitDelegate(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlapDelegate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetDamageBox() const;

	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetLeftHurtBox() const;

	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetRightHurtBox() const;

	UFUNCTION(BlueprintCallable)
	int GetPointWorth() const;

private:
	UPROPERTY(EditAnywhere, Category = Points)
	int PointWorth = 100;
	
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* DamageBox;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* LeftHurtBox;
	
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* RightHurtBox;

	UFUNCTION(BlueprintCallable)
	virtual void GivePoints(APC_PlayerFox* PlayerChar) override;
};
