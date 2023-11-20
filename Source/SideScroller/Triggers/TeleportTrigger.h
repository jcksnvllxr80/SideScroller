// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "TeleportTrigger.generated.h"

class ADoor;
/**
 * 
 */
UCLASS()
class SIDESCROLLER_API ATeleportTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ATeleportTrigger();
	
	UFUNCTION(BlueprintCallable)
	void Teleport(APC_PlayerFox* Player);

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TeleportTargetLocation;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere)
	float TeleportDelayTimer = 1.f;

	UPROPERTY(EditAnywhere)
	ADoor* TeleportSourceDoor;
	
	UPROPERTY(EditAnywhere)
	FVector GlobalTeleportTargetLocation;

	UPROPERTY(VisibleAnywhere)
	bool bPlayerIsOverlappingTrigger;
	
	UPROPERTY(VisibleAnywhere)
	const APC_PlayerFox* OverlappingPlayer;
	
	FTimerHandle TeleportDelayTimerHandle;

	FTimerDelegate TeleportDelayDelegate;

	UPROPERTY(EditAnywhere)
	USoundBase* TeleportSound;

	UFUNCTION(BlueprintCallable)
	void PlayTeleportSound(const APC_PlayerFox* Player);
	
	UFUNCTION(BlueprintCallable)
	void PrepForTeleport(const APC_PlayerFox* Player);

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
