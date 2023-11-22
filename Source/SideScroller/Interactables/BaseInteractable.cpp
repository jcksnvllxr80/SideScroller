// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseInteractable.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

ABaseInteractable::ABaseInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
	InteractableFlipbook->SetupAttachment(RootComponent);

	this->InteractPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractPromptWidget"));
	this->InteractPrompt->SetupAttachment(RootComponent);
	
	this->InteractableBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorInteractBox"));
	this->InteractableBox->SetupAttachment(InteractableFlipbook);
	this->InteractableBox->SetHiddenInGame(true);
	
	if (WidgetInteractPrompt != nullptr)
	{
		UUserWidget* InteractPromptWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetInteractPrompt);
		if (InteractPromptWidget != nullptr)
		{
			this->InteractPrompt->SetWidget(InteractPromptWidget);
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("ABaseInteractable::ABaseInteractable - InteractPromptWidget is null. not setting widget comp.")
			)
		}
	}
}

void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	this->InteractableBox->SetGenerateOverlapEvents(true);
	this->InteractableBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractable::OnBeginOverlapDelegate);

	InteractableFlipbook->SetFlipbook(FalsePosition);

	if (this->InteractPrompt->GetWidget() != nullptr)
	{
		this->InteractPrompt->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		this->InteractPrompt->SetRelativeLocation(FVector {0.000000,0.000000,10.000000});
	}
}

bool ABaseInteractable::GetCanInteract() const
{
	return bCanInteract;
}

void ABaseInteractable::SetCanInteract(const bool CanInteract)
{
	this->bCanInteract = CanInteract;
}

void ABaseInteractable::OnBeginOverlapDelegate(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(OtherActor);
	if (PlayerFox == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ABaseInteractable::OnBeginOverlapDelegate - Overlap of %s is %s, which is not a PlayerFox"),
			*this->GetName(),
			*OtherActor->GetName()
		)
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("ABaseInteractable::OnBeginOverlapDelegate - Interactable, %s, overlapped by %s"),
		*this->GetName(),
		*PlayerFox->GetPlayerName().ToString()
	)

	if (this->InteractPrompt->GetWidget() != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("ABaseInteractable::NotifyActorEndOverlap - Displaying interact prompt"))
		this->InteractPrompt->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}
	
	PlayerFox->SetInteractableObject(OverlappedComponent);
}

void ABaseInteractable::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	APC_PlayerFox* PlayerFox = Cast<APC_PlayerFox>(OtherActor);
	if (PlayerFox == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("ABaseInteractable::NotifyActorEndOverlap - Overlap of %s was %s, which was not a PlayerFox"),
			*this->GetName(),
			*OtherActor->GetName()
		)
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("ABaseInteractable::NotifyActorEndOverlap - Interactable, %s, overlap with %s is no longer occuring"),
		*this->GetName(),
		*PlayerFox->GetPlayerName().ToString()
	)


	if (this->InteractPrompt->GetWidget() != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("ABaseInteractable::NotifyActorEndOverlap - Hiding interact prompt"))
		this->InteractPrompt->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}

	PlayerFox->ClearInteractableObject();
}
