// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseInteractable.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

/**
 * Initialize the ABaseInteractable object.
 */
ABaseInteractable::ABaseInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("InteractFlipbook"));
	InteractableFlipbook->SetupAttachment(RootComponent);

	this->InteractPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractPromptWidget"));
	this->InteractPrompt->SetupAttachment(RootComponent);
	
	this->InteractableBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
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
	
	this->SetReplicates(true);
	this->InteractableFlipbook->SetIsReplicated(true);
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * This method is called when the game starts or when the object is spawned. It is an overridden method of the base
 * class AActor.
 * The method performs the following actions:
 * - Calls the Super::BeginPlay() method to execute the BeginPlay method of the base class.
 * - Sets the InteractableBox to generate overlap events.
 * - Adds a dynamic delegate to the InteractableBox's OnComponentBeginOverlap event, which will call the
 * OnBeginOverlapDelegate method.
 * - Sets the InteractableFlipbook to display the FalsePosition flipbook.
 * - Checks if the InteractPrompt widget is not null. If it is not null, it hides the widget and sets its relative
 * location to (0.000000, 0.000000, 10.000000).
 *
 * @param None.
 * @return None.
 */
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

/**
 * @brief Get the value of bCanInteract.
 *
 * @return A boolean indicating whether the interactable object can be interacted with or not.
 */
bool ABaseInteractable::GetCanInteract() const
{
	return bCanInteract;
}

/**
 * \brief Sets the ability to interact with the interactable object.
 *
 * \param CanInteract   The new value indicating if interaction is allowed or not.
 *
 * This method updates the value of the 'bCanInteract' member variable, which determines
 * whether the interactable object can be interacted with or not.
 */
void ABaseInteractable::SetCanInteract(const bool CanInteract)
{
	this->bCanInteract = CanInteract;
}

/**
 * Called when this interactable object begins overlapping with another actor.
 *
 * @param OverlappedComponent The component that is overlapping with another actor.
 * @param OtherActor The actor that is overlapping with this interactable object.
 * @param OtherComp The component of the other actor that is overlapping with this interactable object.
 * @param OtherBodyIndex The body index of the other actor that is overlapping with this interactable object.
 * @param bFromSweep True if this overlap is from a sweep, false otherwise.
 * @param SweepResult The hit result from the overlap sweep.
 */
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

/**
 * Notifies when an actor ends overlapping with this interactable object.
 *
 * @param OtherActor The actor that ended overlapping with this interactable.
 */
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

/**
 * Retrieve the replicated properties for this object's lifetime.
 *
 * This method should be overridden in derived classes to specify the replicated properties that should be
 * synchronized across the network during the lifetime of this object. The lifetime replicated properties are
 * defined using the DOREPLIFETIME macro.
 *
 * @param OutLifetimeProps A reference to an array of FLifetimeProperty objects, which will be populated with the
 *                         replicated properties.
 *
 * @see DOREPLIFETIME
 */
void ABaseInteractable::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseInteractable, bIsTrue);
	DOREPLIFETIME(ABaseInteractable, InteractableFlipbook);
	DOREPLIFETIME(ABaseInteractable, bCanInteract);
}