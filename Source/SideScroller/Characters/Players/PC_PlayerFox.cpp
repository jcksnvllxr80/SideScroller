#include "PC_PlayerFox.h"

#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/GameModes/LevelGameMode.h"
#include "SideScroller/GameModes/SideScrollerGameModeBase.h"
#include "SideScroller/GameStates/LobbyGameState.h"
#include "SideScroller/SaveGames/SideScrollerSaveGame.h"

/**
 * APC_PlayerFox Constructor.
 * Initializes the player's attributes and components.
 */
APC_PlayerFox::APC_PlayerFox()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetWorldLocationAndRotation(
		FVector(0.0f, 0.0f, 50.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);
	CameraArm->bDoCollisionTest = false;
	CameraArm->TargetArmLength = 200.f;
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 5.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm);

	NameBanner = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameBanner"));
	NameBanner->SetupAttachment(RootComponent);
	NameBanner->SetRelativeScale3D(FVector(0.4, 0.4, 0.4));
	NameBanner->SetRelativeLocation(FVector(0.f, 0.1, 10.f));
	NameBanner->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	NameBanner->SetHorizontalAlignment(EHTA_Center);
	NameBanner->SetTextRenderColor(FColor(0, 11, 133, 255));

	this->GetCharacterMovement()->MaxWalkSpeed = 250.0;
	this->GetCharacterMovement()->JumpZVelocity = 525.0;
	this->GetCharacterMovement()->GravityScale = 3.5;
	this->GetCharacterMovement()->AirControl = 0.4;
	this->GetCharacterMovement()->MaxStepHeight = 10.0;
	this->GetCharacterMovement()->SetWalkableFloorAngle(60.f);
	this->GetCharacterMovement()->MaxAcceleration = 400.f;
	this->GetCharacterMovement()->BrakingFrictionFactor = 0.65;

	this->GetSprite()->SetIsReplicated(true);
	this->GetCharacterMovement()->SetIsReplicated(true);
	this->SetReplicates(true);
	this->CurrentRotation = MovingLeftRotation;
	this->LastRotation = this->CurrentRotation;
}

/**
 * @brief The BeginPlay method is called when the game starts or when the level is being replayed. It initializes
 * various variables and sets up the player's HUD and game messages.
 *
 * @details The BeginPlay method performs the following operations:
 * <ul>
 *    <li>Calls the base class implementation of BeginPlay.</li>
 *    <li>Obtains a reference to the game instance and stores it in the GameInstance variable.</li>
 *    <li>Adds the player to the players array.</li>
 *    <li>Loads the player's profile name.</li>
 *    <li>Performs setup for the player's HUD.</li>
 *    <li>Performs setup for the player's game messages.</li>
 *    <li>Sets the NameBanner text to the player's name.</li>
 *    <li>Stores the current location of the player's sprite in the LastCheckpointLocation variable.</li>
 *    <li>Stores the braking friction factor and maximum walk speed of the player's character movement component in the
 *    StandingFriction and NormalWalkingSpeed variables respectively.</li>
 *    <li>Sets a timer to call the DoLevelWelcome method after a specified delay.</li>
 * </ul>
 */
void APC_PlayerFox::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	AddToPlayersArray();
	LoadProfilePlayerName();
	PlayerHUDSetup();
	PlayerGameMessageSetup();

	this->NameBanner->SetText(GetPlayerName());
	this->LastCheckpointLocation = this->GetSprite()->GetComponentLocation(); 
	this->StandingFriction = this->GetCharacterMovement()->BrakingFrictionFactor;
	this->NormalWalkingSpeed = this->GetCharacterMovement()->MaxWalkSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		this->LevelWelcomeDelayTimerHandle,
		this,
		&APC_PlayerFox::DoLevelWelcome,
		LevelWelcomeDelayTime,
		false
	);
}

void APC_PlayerFox::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CumulativeTime += DeltaTime;
	// if (CumulativeTime > 0.5f)
	// {
	// 	LogSpeed();
	// 	LogRotation();
	// 	LogLocation();
	// 	CumulativeTime = 0.f;
	// }
	
	UpdateAnimation();
	UpdateNameBanner();
}

/**
 * Set up the input bindings for the player controller.
 *
 * This method is responsible for binding player input events to specific
 * functions in the player controller. It sets up the input component and
 * binds the necessary axis and action events.
 *
 * @param PlayerInputComponent The input component to set up.
 */
void APC_PlayerFox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APC_PlayerFox::MoveRight);
	PlayerInputComponent->BindAxis("ClimbUp", this, &APC_PlayerFox::ClimbUpAxisInputCallback);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APC_PlayerFox::Jump);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APC_PlayerFox::Shoot);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &APC_PlayerFox::UseAction);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APC_PlayerFox::SetRunVelocity);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APC_PlayerFox::SetWalkVelocity);
	PlayerInputComponent->BindAction("InGameContextMenu", IE_Pressed, this, &APC_PlayerFox::OpenMenu);
	PlayerInputComponent->BindAction("SpectateNextPlayer", IE_Pressed, this, &APC_PlayerFox::SpectateNextPlayer);
	PlayerInputComponent->BindAction("SpectatePrevPlayer", IE_Pressed, this, &APC_PlayerFox::SpectatePrevPlayer);
}

/**
 * @brief Displays a level welcome message.
 *
 * This method is responsible for displaying a level welcome message if the game state is a SideScrollerGameState and
 * the current level is identified as a level.
 *
 * @param None
 *
 * @return None
 */
void APC_PlayerFox::DoLevelWelcome()
{
	const ASideScrollerGameState* GameState = Cast<ASideScrollerGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("APC_PlayerFox::DoLevelWelcome - Not a SideScrollerGameState. Not displaying level welcome message.")
		);
		return;
	}

	UE_LOG(LogTemp, Display,
		TEXT("APC_PlayerFox::DoLevelWelcome - GameState is %s."),
		*GetWorld()->GetGameState()->GetName()
	);
	
	if (GetWorld()->GetGameState()->GetName().Contains("Level"))
	{
		UE_LOG(LogTemp, Display, TEXT("APC_PlayerFox::DoLevelWelcome - Displaying level welcome message."))
		
		const FString GameMessage = FString::Printf( TEXT("Level %i Begin!"), GameState->GetCurrentLevel());
		DisplayGameMessage(FText::FromString(GameMessage));
		
		UGameplayStatics::SpawnSoundAttached(
			this->LevelStartSound,
			this->GetSprite(),
			TEXT("StartLevelSound")
		);

		GetWorld()->GetTimerManager().SetTimer(
			this->LevelStartMessageTimerHandle,
			this,
			&APC_PlayerFox::HideGameMessage,
			LevelStartMessageTime,
			false
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("APC_PlayerFox::DoLevelWelcome - This is not a Level. Not displaying level welcome message.")
		);
	}
}

/**
 * Loads the player name from the game instance and sets it as the player's name.
 * If the game instance is null, the player name is set to the object's name.
 * If the world is null, no further action is taken.
 * If the local role is either ROLE_Authority or ROLE_AutonomousProxy and the controller is the first player controller
 * in the world:
 *   - If the player name is not already set, it is retrieved from the game instance's player profile and the
 *   bPlayerNameSet flag is set to true.
 *   - If the player name has not been sent to the server, and the local role is ROLE_AutonomousProxy, the player
 *   name is sent to the server using SendPlayerNameToServer() and the bPlayerNameSentToServer flag is set to true.
 *
 * @param None
 */
void APC_PlayerFox::LoadProfilePlayerName()
{
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("APC_PlayerFox::LoadProfilePlayerName - No GameInstance. Using default PlayerName")
		);

		this->PlayerName = this->GetName();
		return;  // no game instance - early return
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_PlayerFox::LoadProfilePlayerName - Not getting name. Cant find World."))
		return;  // dont go any further, cant find world
	}
	
	const ENetRole CurrentRole = GetLocalRole();
	if (Cast<APlayerController>(this->GetController()) == World->GetFirstPlayerController())
	{
		if (!bPlayerNameSet)
		{
			if (CurrentRole == ROLE_Authority || CurrentRole == ROLE_AutonomousProxy)
			{
				this->PlayerName = GameInstance->GetPlayerProfile()->PlayerName;
				this->bPlayerNameSet = true;
			}
		}

		if (!bPlayerNameSentToServer)
		{
			if (CurrentRole == ROLE_AutonomousProxy)
			{
				SendPlayerNameToServer(this->PlayerName);
				this->bPlayerNameSentToServer = true;
			}
		}
	}
}

/**
 * Retrieves the properties that should be replicated for this character's lifetime.
 *
 * @param OutLifetimeProps - The array that will contain the replicated properties.
 */
void APC_PlayerFox::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APC_PlayerFox, SpectatorsStr);
	DOREPLIFETIME(APC_PlayerFox, PlayerBeingSpectated);
	DOREPLIFETIME(APC_PlayerFox, Spectators);
	DOREPLIFETIME(APC_PlayerFox, bIsClimbing);
	DOREPLIFETIME(APC_PlayerFox, bIsCrouching);
	DOREPLIFETIME(APC_PlayerFox, bIsSliding);
	DOREPLIFETIME(APC_PlayerFox, bOnLadder);
	DOREPLIFETIME(APC_PlayerFox, CurrentRotation);
	DOREPLIFETIME(APC_PlayerFox, PlayerName);
	DOREPLIFETIME(APC_PlayerFox, WalkSound);
	DOREPLIFETIME(APC_PlayerFox, AccumulatedPoints);
	DOREPLIFETIME(APC_PlayerFox, NumberOfLives);
	DOREPLIFETIME(APC_PlayerFox, bIsOutOfLives);
}

int APC_PlayerFox::GetAccumulatedPoints() const
{
	return this->AccumulatedPoints;
}

void APC_PlayerFox::SetAccumulatedPoints(const int Points)
{
	this->AccumulatedPoints = Points;
}

int APC_PlayerFox::GetNumberOfLives() const
{
	return this->NumberOfLives;
}

void APC_PlayerFox::SetNumberOfLives(const int NumLives)
{
	this->NumberOfLives = NumLives;
}

int APC_PlayerFox::GetCherryCount() const
{
	return this->CherryStash;
}

void APC_PlayerFox::SetCherryStash(int NumCherries)
{
	this->CherryStash = NumCherries;
}

int APC_PlayerFox::GetMoneyCount() const
{
	return this->MoneyStash;
}

void APC_PlayerFox::SetMoneyStash(const int MoneyAmount)
{
	this->MoneyStash = MoneyAmount;
}

void APC_PlayerFox::SetLastCheckpointLocation(const FVector& Location)
{
	this->LastCheckpointLocation = Location;
}

/**
 * @brief Prints the list of players.
 *
 * This method takes an array of APC_PlayerFox objects and prints the list of players.
 * The players' names and statuses (dead or alive) are concatenated into a string,
 * and the string is logged in the Unreal Engine's log window using the Display level.
 * If a null player is encountered in the array, a log message is displayed indicating
 * that a null player was found, and the loop continues to the next player.
 *
 * @param PlayersArray The array of APC_PlayerFox objects representing the list of players.
 */
void APC_PlayerFox::PrintPlayersList(TArray<APC_PlayerFox*> PlayersArray)
{
	FString PlayerArrayStr = "";
	for (const APC_PlayerFox* Player : PlayersArray)
	{
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Display,
				TEXT("APC_PlayerFox::PrintPlayersList - Found null Player.")
			);
			continue;
		}
		PlayerArrayStr += (Player->GetName() + (Player->IsDead() ? ": Dead; " : ": Alive; "));
	}
	UE_LOG(LogTemp, Display, TEXT("List of Players is %s"), *PlayerArrayStr);
}

UPrimitiveComponent* APC_PlayerFox::GetInteractableObject() const
{
	return InteractableObject;
}

void APC_PlayerFox::SetInteractableObject(UPrimitiveComponent* InteractableObj)
{
	this->InteractableObject = InteractableObj;
}

void APC_PlayerFox::ClearInteractableObject()
{
	this->InteractableObject = nullptr;
}

/**
 * @brief Multicast RPC called when the level is complete for the player character control.
 *
 * This function is responsible for displaying the level complete message, playing the level complete sound,
 * and hiding the level complete message after a certain time.
 *
 * @param None
 *
 * @return None
 */
void APC_PlayerFox::DoLevelCompleteMulticastRPC_Implementation()
{
	const ASideScrollerGameState* GameState = Cast<ASideScrollerGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("APC_PlayerFox::DoLevelComplete - Not a SideScrollerGameState. Not displaying level complete message.")
		);
		return;
	}
	
	const FString GameMessage = FString::Printf( TEXT("Level %i Complete!"), GameState->GetCurrentLevel());
	DisplayGameMessage(FText::FromString(GameMessage));
		
	UGameplayStatics::SpawnSoundAttached(
		this->LevelCompleteSound,
		this->GetSprite(),
		TEXT("CompleteLevelSound")
	);

	GetWorld()->GetTimerManager().SetTimer(
		this->LevelCompleteMessageTimerHandle,
		this,
		&APC_PlayerFox::HideGameMessage,
		LevelCompleteMessageTime,
		false
	);
}

void APC_PlayerFox::DoLevelCompleteServerRPC_Implementation()
{
	DoLevelCompleteMulticastRPC();
}

/**
 * Sets a player as the current player being spectated by this player.
 *
 * @param Player The player to be spectated.
 * @return Whether the player was successfully set as the current player being spectated.
 */
bool APC_PlayerFox::FoundPlayerToSpectate(APC_PlayerFox* Player)
{
	if (Player != nullptr && Player != this && !Player->IsDead()) {
		if (this->PlayerBeingSpectated != nullptr)
		{
			UE_LOG(LogTemp, Display,
				TEXT("Removing %s from %s's spectator list."),
				*this->GetPlayerName().ToString(),
				*this->PlayerBeingSpectated->GetPlayerName().ToString()
			);
			this->PlayerBeingSpectated->RemoveFromSpectators(this);
		}
		this->PlayerBeingSpectated = Player;
		
		UE_LOG(LogTemp, Display,
			TEXT("Adding %s to %s's spectator list."),
			*this->GetPlayerName().ToString(),
			*this->PlayerBeingSpectated->GetPlayerName().ToString()
		);

		this->PlayerBeingSpectated->AddToSpectators(this);
		Spectate();
		return true;
	}
	return false;
}

/**
 * @brief Begin spectating a player in the game.
 *
 * This method starts the spectating process by searching for a player to spectate.
 * The method takes in a pointer to the game mode and a flag indicating whether to search in reverse order.
 *
 * If the search is not in reverse order, the method iterates through the players array obtained from the game mode
 * and calls the FoundPlayerToSpectate method for each player until a suitable player is found.
 *
 * If the search is in reverse order, the method starts at the last element of the players array and iterates backwards
 * until the first element, calling the FoundPlayerToSpectate method for each player until a suitable player is found.
 *
 * Note that the FoundPlayerToSpectate method should be implemented separately and is not shown in this code sample.
 *
 * @param GameMode A pointer to the game mode.
 * @param SearchInReverse Flag indicating whether to search in reverse order.
 */
void APC_PlayerFox::BeginSpectating(const ASideScrollerGameModeBase* GameMode, const bool SearchInReverse = false)
{
	TArray<APC_PlayerFox*> PlayersArray = GameMode->GetPlayers();
	PrintPlayersList(PlayersArray);
	
	if (!SearchInReverse)
	{
		for (APC_PlayerFox* Player : PlayersArray)
		{
			if (FoundPlayerToSpectate(Player)) break;
		}
	} else {
		for (int i = PlayersArray.Num() - 1; i >= 0; i--)
		{
			APC_PlayerFox* Player = PlayersArray[i];
			if (FoundPlayerToSpectate(Player)) break;
		}
	}
}

/**
 * @brief Spectate the other player.
 *
 * This method allows the player to spectate the other player in the game.
 * It first checks if the current game mode is an instance of ASideScrollerGameModeBase,
 * and then calls the BeginSpectating method passing the game mode instance as argument.
 *
 */
void APC_PlayerFox::SpectateOtherPlayer()
{
	if (const ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode()
		)
	) {
		BeginSpectating(GameMode);
	}
}

/**
 * @brief Spectates the next player in the game.
 *
 * If the current player is dead, it will attempt to spectate the next available player.
 *
 * @details This method is part of the APC_PlayerFox class, which inherits from a base class called
 * ASideScrollerGameModeBase.
 *
 * @note This method assumes that the current player is already dead and will only proceed to spectate the next player
 * if there is one available.
 */
void APC_PlayerFox::SpectateNextPlayer()
{
	if (const ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode()
		)
	) {
		if (this->IsDead())
		{
			const APC_PlayerFox* LastSpectatedPlayer = this->PlayerBeingSpectated;
			TArray<APC_PlayerFox*> PlayersArray = GameMode->GetPlayers();
			PrintPlayersList(PlayersArray);
			
			if (int CurrentlySpectatingIndex = PlayersArray.IndexOfByKey(LastSpectatedPlayer);
				++CurrentlySpectatingIndex < PlayersArray.Num()
			) {
				do
				{
					APC_PlayerFox* Player = PlayersArray[CurrentlySpectatingIndex];
					if (FoundPlayerToSpectate(Player)) break;
				} while (++CurrentlySpectatingIndex < PlayersArray.Num());
			}
			
			if (LastSpectatedPlayer == this->PlayerBeingSpectated) {
				BeginSpectating(GameMode);
			}
		}
	}
}

/**
 * @brief Spectate the player's spectable character.
 *
 * This method sets the player's view target to the character being spectated. It requires the player to have a valid
 * controller and the spectable character to have a valid controller and view target.
 *
 * @param None
 *
 * @return None
 *
 * @note This method will log a warning if the player's controller is not found, cannot be cast to a PlayerController,
 * or if the spectable character's controller or view target is not found. It will log an error if there is an
 * exception when setting the view target.
 */
void APC_PlayerFox::Spectate() const
{
	AController* ThisController = this->GetController();
	if (ThisController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_PlayerFox::Spectate - Cant find ThisController"))
		return;
	}
	
	APlayerController* PlayerController = dynamic_cast<APlayerController*>(ThisController);
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_PlayerFox::Spectate - Cant cast Controller to PlayerController"))
		return;
	}

	const AController* SpectableController = this->PlayerBeingSpectated->GetController();
	if (SpectableController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_PlayerFox::Spectate - Cant find SpectableController"))
		return;
	}

	AActor* SpectableActor = SpectableController->GetViewTarget();
	if (SpectableActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_PlayerFox::Spectate - Cant find SpectableActor"))
		return;
	}

	try
	{
		PlayerController->SetViewTargetWithBlend(SpectableActor);
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("APC_PlayerFox::Spectate - Catch block - cant SetViewTargetWithBlend"))
	}
}

/**
 * Spectate the previous player.
 *
 * This method allows the player to spectate the previous player in the game. It checks if the current player is dead
 * and retrieves the game mode object. If the current player is dead and the game mode object is valid, the method
 * finds the index of the player that was last being spectated and iterates through the players array starting from
 * that index, to find the previous player that can be spectated. Once a suitable player is found, the method tries
 * to spectate that player. If no previous player is found, or if the player being spectated remains the same, the
 * method resumes normal spectator mode.
 *
 * @param None.
 * @return None.
 */
void APC_PlayerFox::SpectatePrevPlayer()
{
	if (const ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode()
		)
	) {
		if (this->IsDead())
		{
			const APC_PlayerFox* LastSpectatedPlayer = this->PlayerBeingSpectated;
			TArray<APC_PlayerFox*> PlayersArray = GameMode->GetPlayers();
			PrintPlayersList(PlayersArray);
			
			if (int CurrentlySpectatingIndex = PlayersArray.IndexOfByKey(LastSpectatedPlayer);
				--CurrentlySpectatingIndex >= 0
			) {
				do
				{
					APC_PlayerFox* Player = PlayersArray[CurrentlySpectatingIndex];
					if (FoundPlayerToSpectate(Player)) break;
				} while (--CurrentlySpectatingIndex < PlayersArray.Num());
			} 
	
			if (LastSpectatedPlayer == this->PlayerBeingSpectated) {
				BeginSpectating(GameMode, true);
			}
		}
	}
}

/**
 * Executes the use action for the player character.
 *
 * @brief Use the currently interactable object if there is one.
 *
 * @details This method checks if there is an interactable object available for the player character to interact with.
 * If there is, it calls the Interact() method on the object implementing the IInteractInterface.
 * If there isn't, it logs a message indicating that there is no item to interact with.
 *
 * @param None.
 *
 * @return None.
 */
void APC_PlayerFox::UseAction_Implementation()
{
	if (InteractableObject == nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::UseAction - No item for %s to interact with."),
			*this->GetPlayerName().ToString()
		)
		return;
	}
	
	IInteractInterface* InteractableInterfaceObject = nullptr;
	if (InteractableObject->GetOwner()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		InteractableInterfaceObject = Cast<IInteractInterface>(InteractableObject->GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::UseAction - %s is not an interactableobject."),
			*InteractableObject->GetName()
		)
	}
	
	if (InteractableInterfaceObject != nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::UseAction - %s is interacting with %s."),
			*this->GetPlayerName().ToString(),
			*InteractableObject->GetName()
		)
		InteractableInterfaceObject->Interact();
	}
	else
	{
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::UseAction - No item for %s to interact with."),
			*this->GetPlayerName().ToString()
		)
	}
}

bool APC_PlayerFox::UseAction_Validate()
{
	return true;
}

/**
 * Moves all spectators to a new player.
 *
 * @param this - The current player.
 */
void APC_PlayerFox::MoveSpectatorsToNewPlayer() const
{
	TArray<APC_PlayerFox*> PlayerSpectators = this->GetSpectators();
	for (APC_PlayerFox* PlayerSpectator : PlayerSpectators)
	{
		PlayerSpectator->SpectateNextPlayer();
	}
}

/**
 * @brief Revives the player character at the last checkpoint.
 *
 * This method sets the player character's health to its default value and teleports it to
 * the last checkpoint location. It also stops the player character's movement.
 *
 * @param None.
 *
 * @return None.
 */
void APC_PlayerFox::ReviveAtCheckpoint()
{
	// set location back to last checkpoint
	this->SetHealth(this->DefaultHealth);
	this->SetActorLocation(
		LastCheckpointLocation, false, nullptr, ETeleportType::ResetPhysics
	);
	this->GetMovementComponent()->StopMovementImmediately();
}

/**
 * @brief Implements the functionality for a player's death.
 *
 * This method is called when a player dies. It reduces the number of lives
 * the player has by one, and then revives the player at the checkpoint.
 * If the player has no remaining lives, the player is removed from the game,
 * the death animation is triggered, and the player is set to spectate the next player.
 */
void APC_PlayerFox::PlayerDeath_Implementation()
{
	if (this->NumberOfLives > 0)
	{
		// take a life away
		this->NumberOfLives -= 1;
		GameInstance->RespawnLoadMenu();
	} else {
		this->RemoveFromPlayersArray();
		this->DoDeath();

		this->bIsOutOfLives = true;
		SpectateNextPlayer();  // SpectateOtherPlayer();
	}
}

bool APC_PlayerFox::PlayerDeath_Validate()
{
	return true;
}

void APC_PlayerFox::HandleFallOffLevel()
{
	// Maybe one color character doesnt die when it falls off, wrap PlayerDeath in that case.
	PlayerDeath();
}

/**
 * @brief Performs the walk animation and plays a walking sound.
 *
 * This method is responsible for updating the character's sprite flipbook to the run animation
 * if it is different from the current flipbook, and playing a walking sound every `FramesPerStep` frames.
 *
 * @note This method assumes that `RunAnimation` is a valid flipbook and `WalkSound` is a valid sound.
 *
 * @param None.
 *
 * @return None.
 *
 * @see GetSprite, SetFlipbook, GetPlaybackPositionInFrames, FramesPerStep, UGameplayStatics::SpawnSoundAttached
 */
void APC_PlayerFox::DoWalkAnimAndSound()
{
	if (this->GetSprite()->GetFlipbook() != RunAnimation) {
		this->GetSprite()->SetFlipbook(RunAnimation);
	}
			
	if (this->GetSprite()->GetPlaybackPositionInFrames() % this->FramesPerStep == 0) {
		// UE_LOG(LogTemp, VeryVerbose, TEXT("Playing %s's walking sound!"), *this->GetName());
		UGameplayStatics::SpawnSoundAttached(
			this->WalkSound,
			this->GetSprite(),
			TEXT("PaperCharacterSpriteWalk")
		);
	}
}

/**
 * This method is responsible for playing the appropriate climb animation and sound.
 *
 * @param None
 */
void APC_PlayerFox::DoClimbAnimAndSound()
{
	if (this->GetVelocity().Z == 0) {
		if (this->GetSprite()->GetFlipbook() != StopOnLadderAnimation) {
			this->GetSprite()->SetFlipbook(StopOnLadderAnimation);
		}
	} else {
		if (this->GetSprite()->GetFlipbook() != ClimbAnimation) this->GetSprite()->SetFlipbook(ClimbAnimation);
		if (this->GetSprite()->GetPlaybackPositionInFrames() == 0) {
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Playing %s's climbing sound!"), *this->GetName());
			UGameplayStatics::SpawnSoundAttached(
				this->NearbyClimbableSound,
				this->GetSprite(),
				TEXT("ClimbingSound")
			);
		}
	}
}

void APC_PlayerFox::AddToPlayersArray()
{
	if (ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode())
	) {
		GameMode->AddPlayer(this);	
	}
}

void APC_PlayerFox::RemoveFromPlayersArray()
{
	if (ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode())
	) {
		GameMode->RemovePlayer(this);	
	}
}

void APC_PlayerFox::PlayerHUDSetup()
{
	if (WidgetPlayerHUD) {
		this->WidgetPlayerHUDInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetPlayerHUD);
		this->WidgetPlayerHUDInstance->AddToViewport();
	}
}

/**
 * @brief Tears down the player HUD widget instance.
 *
 * This method removes the player HUD widget instance from its parent. If the widget instance
 * is already null, a warning message will be logged.
 *
 * @param None
 *
 * @return None
 */
void APC_PlayerFox::PlayerHUDTeardown()
{
	if (this->WidgetPlayerHUDInstance != nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::PlayerHUDTeardown - Tearing down WidgetPlayerHUDInstance")
		)
		this->WidgetPlayerHUDInstance->RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("APC_PlayerFox::PlayerHUDTeardown - WidgetPlayerHUDInstance is null")
		)
	}
}

/**
 * @brief Tears down the PlayerMessageWidget.
 *
 * This method removes the WidgetPlayerGameMessageInstance from the parent if it is not null. If the
 * WidgetPlayerGameMessageInstance is null, a warning log is displayed.
 *
 * @param None.
 * @return None.
 */
void APC_PlayerFox::PlayerMessageWidgetTeardown()
{
	if (this->WidgetPlayerGameMessageInstance != nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::PlayerMessageWidgetTeardown - Tearing down WidgetPlayerGameMessageInstance")
		)
		this->WidgetPlayerGameMessageInstance->RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("APC_PlayerFox::PlayerMessageWidgetTeardown - WidgetPlayerGameMessageInstance is null")
		)
	}
}

/**
 * @brief Sets up the player's game messages.
 *
 * This method creates and adds the player's game message widget to the viewport. It also hides the game message.
 *
 * @param WidgetPlayerGameMessage The widget template to use for the player's game message.
 */
void APC_PlayerFox::PlayerGameMessageSetup()
{
	if (WidgetPlayerGameMessage) {
		this->WidgetPlayerGameMessageInstance = CreateWidget<UUserWidget>(
			GetWorld(), WidgetPlayerGameMessage
		);
		this->WidgetPlayerGameMessageInstance->AddToViewport();
		HideGameMessage();
	}
}

/**
 * Retrieves the text block widget for displaying game messages.
 *
 * @param TextBlock A reference to a UTextBlock pointer to hold the retrieved text block widget.
 */
void APC_PlayerFox::GetMessageWidgetTextBlock(UTextBlock*& TextBlock) const
{
	if (this->WidgetPlayerGameMessageInstance != nullptr)
	{
		const FName TextBlockName = "UserMessageTB";
		TextBlock = Cast<UTextBlock>(
			this->WidgetPlayerGameMessageInstance->GetWidgetFromName(TextBlockName)
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("APC_PlayerFox::GetMessageWidgetTextBlock - cant get game message text block. no widget.")
		);
	}
}

/**
 * Displays a game message using the given text.
 *
 * @param Message The text to display as the game message.
 *
 * @return None.
 */
void APC_PlayerFox::DisplayGameMessage(FText Message)
{
	UTextBlock* TextBlock;
	GetMessageWidgetTextBlock(TextBlock);

	if (TextBlock != nullptr)
	{
		TextBlock->SetText(Message);
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::DoLevelWelcome - Displaying level welcome message, %s."),
			*Message.ToString()
		);
		TextBlock->SetVisibility(ESlateVisibility::Visible);
	}
}

/**
 * Hides the game message displayed to the player.
 *
 * @param None
 * @return None
 */
void APC_PlayerFox::HideGameMessage() const
{

	UTextBlock* TextBlock;
	GetMessageWidgetTextBlock(TextBlock);

	if (TextBlock != nullptr)
	{
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::DoLevelWelcome - Hiding user message.")
		);
		TextBlock->SetVisibility(ESlateVisibility::Hidden);
	}

}

void APC_PlayerFox::SendVerticalAxisInputToServer_Implementation(const float Z)
{
	ClimbUpAxisInputCallback(Z);
}

bool APC_PlayerFox::SendVerticalAxisInputToServer_Validate(const float Z)
{
	return true;
}

void APC_PlayerFox::SendRotationToServer_Implementation(const float Value)
{
	UpdateRotation(Value);
}

bool APC_PlayerFox::SendRotationToServer_Validate(const float Value)
{
	return true;
}

void APC_PlayerFox::SendPlayerNameToServer_Implementation(const FString& ClientPlayerName)
{
	this->PlayerName = ClientPlayerName;
}

bool APC_PlayerFox::SendPlayerNameToServer_Validate(const FString& ClientPlayerName)
{
	return ClientPlayerName != "";
}

/**
 * @brief Clean up when the player character dies.
 *
 * This method removes the player character from the players array, tears down the player HUD,
 * tears down the player message widget, and moves spectators to a new player.
 */
void APC_PlayerFox::DeathCleanUp()
{
	this->RemoveFromPlayersArray();
	this->PlayerHUDTeardown();
	this->PlayerMessageWidgetTeardown();
	this->MoveSpectatorsToNewPlayer();
}

/**
 * Update the animation of the player character based on their current state and movement speed.
 *
 * @param None
 * @return None
 */
void APC_PlayerFox::UpdateAnimation()
{
	if (this)
	{
		// early return if player in hurt animation right now
		if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
		
		//Update our falling property
		bIsFalling = this->GetMovementComponent()->IsFalling();
 
		//Update our movement speed
		const float MovementSpeed = this->GetVelocity().Size();

		if (bIsFalling) {
			if (this->GetVelocity().Z > 0.f) {
				this->GetSprite()->SetFlipbook(JumpAnimation);
			}
			else {
				this->GetSprite()->SetFlipbook(FallAnimation);
			}
		}
		else if (this->bIsCrouching) {
			this->GetSprite()->SetFlipbook(CrouchAnimation);
		} else if (this->bIsClimbing || this->bOnLadder) {
			DoClimbAnimAndSound();
		}
		else if (MovementSpeed != 0.f) {
			DoWalkAnimAndSound();
		}
		else {
			this->GetSprite()->SetFlipbook(IdleAnimation);
		}
	}
}

/**
 * Updates the rotation of the player character based on the input value.
 *
 * @param Value The input value determining the rotation direction (-1 for left, 0 for no rotation, 1 for right).
 */
void APC_PlayerFox::UpdateRotation(const float Value)
{
	const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
	
	if (Value < 0) {
		this->CurrentRotation = MovingLeftRotation;
		this->GetSprite()->SetRelativeRotation(MovingLeftRotation);
		GetProjectileSpawnPoint()->SetRelativeLocation(
			FVector(ProjectileSpawnLoc.X * -1.f, ProjSpawnLoc.Y, ProjSpawnLoc.Z)
		);
	}
	else if (Value > 0){
		this->CurrentRotation = MovingRightRotation;
		GetProjectileSpawnPoint()->SetRelativeLocation(FVector(ProjectileSpawnLoc.X, ProjSpawnLoc.Y, ProjSpawnLoc.Z));
	}
	this->GetSprite()->SetRelativeRotation(CurrentRotation);
	
	// UE_LOG(
	// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
	// 	*this->GetName(),
	// 	*this->GetActorLocation().ToString(),
	// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
	// );

	if (this->LastRotation != this->CurrentRotation)
	{
		this->LastRotation = this->CurrentRotation;
		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			SendRotationToServer(Value);
		}
	}
}

/**
 * @brief Update the name banner of the player.
 *
 * This method updates the name banner text based on the player's name.
 * If the current name banner text is empty, it loads the player's profile name and sets it as the name banner text.
 *
 * @param None
 * @return None
 */
void APC_PlayerFox::UpdateNameBanner()
{
	if (NameBanner->Text.EqualTo(FText::FromString("")))
	{
		LoadProfilePlayerName();
		NameBanner->SetText(FText::FromString(this->PlayerName));
	}
}

/**
 * Sets the value of the bOverlappingClimbable member variable and performs additional actions based on the provided
 * parameters.
 *
 * @param OverlappingClimbable The new value for the bOverlappingClimbable member variable.
 * @param OverlappedClimbable Pointer to the ABaseClimbable object that is being overlapped.
 */
void APC_PlayerFox::SetOverlappingClimbable(bool OverlappingClimbable, ABaseClimbable* OverlappedClimbable)
{
	bOverlappingClimbable = OverlappingClimbable;
	NearbyClimbableSound = OverlappedClimbable->LadderSound;
	if (!bOverlappingClimbable)
	{
		this->bOnLadder = false;
		this->bIsClimbing = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

/**
 * Sets the transform of a projectile based on the given parameters.
 *
 * @param Direction The horizontal direction in which the projectile should move.
 * @param MyOwner The owner of the projectile.
 * @param BaseChar The base character associated with the projectile.
 * @param Projectile The projectile whose transform needs to be set.
 */
void APC_PlayerFox::SetProjectileTransform(
	const float Direction,
	AActor* MyOwner,
	const ABasePaperCharacter* BaseChar,
	const ABaseProjectile* Projectile
) {
	if (ShootUpward)
	{
		Projectile->GetProjectileFlipbook()->SetRelativeRotation(FRotator( 90, 0, 0));
		Projectile->GetProjectileMovementComp()->Velocity = FVector(
			0.f, 0.f, Projectile->GetMovementSpeed()
		);
	}
	else
	{
		Projectile->GetProjectileFlipbook()->SetRelativeRotation(BaseChar->GetSprite()->GetRelativeRotation());
		Projectile->GetProjectileMovementComp()->Velocity = FVector(
			Direction * Projectile->GetMovementSpeed(), 0.f, 0.f
		);
	}
}

/**
 * CrouchClimbDown method is used to handle the logic of crouching and climbing down.
 * It checks various conditions and performs the necessary actions based on those conditions.
 *
 * @param void
 * @return void
 */
void APC_PlayerFox::CrouchClimbDown()
{
	// Run slide ////////////
	const float VelocityX = this->GetVelocity().X;
	if (!this->bIsSliding && (abs(VelocityX) > this->CrouchSlidingThresholdVelocity))
	{
		// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
		// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide (run) to true"));
		UE_LOG(LogTemp, Display,
			TEXT("APC_PlayerFox::CrouchClimbDown - setting bIsSliding to true for run.")
		);
		this->bIsSliding = true;
		this->GetCharacterMovement()->BrakingFrictionFactor = this->CrouchSlideFriction;
		return;
	}
	/////////////////////////
	
	// Hill sliding ////////////
	const float FloorAngleDeg = GetFloorAngle();
	if (!this->bIsSliding)
	{
		if ((FloorAngleDeg <= -SlideAngleDeg) && VelocityX > 0)
		{
			// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Floor Angle = %f"), FloorAngleDeg);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide (hill) to true"));
			UE_LOG(LogTemp, Verbose,
				TEXT("APC_PlayerFox::CrouchClimbDown - RIGHT - setting bIsSliding true - hill.")
			);
			this->bIsSliding = true;
			this->GetCharacterMovement()->BrakingFrictionFactor = 0.f;  // this->CrouchSlideFriction;
			return;
		}

		if ((FloorAngleDeg >= SlideAngleDeg) && VelocityX < 0)
		{
			// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Floor Angle = %f"), FloorAngleDeg);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide (hill) to true"));
			UE_LOG(LogTemp, Verbose,
				TEXT("APC_PlayerFox::CrouchClimbDown - LEFT - setting bIsSliding true - hill.")
			);
			this->bIsSliding = true;
			this->GetCharacterMovement()->BrakingFrictionFactor = 0.f;  // this->CrouchSlideFriction;
			return;
		}
	}
	/////////////////////////

	// Stop hill sliding ////////////
	if (this->bIsSliding && (abs(VelocityX) < CrouchSlidingThresholdVelocity))
	{
		if (FloorAngleDeg < 1.f && FloorAngleDeg > -SlideAngleDeg && VelocityX > 0)
		{
			// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Floor Angle = %f"), FloorAngleDeg);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide to false"));
			UE_LOG(LogTemp, Verbose,
				TEXT("APC_PlayerFox::CrouchClimbDown - RIGHT - setting bIsSliding to false.")
			);
			this->bIsSliding = false;
			this->GetCharacterMovement()->BrakingFrictionFactor = this->StandingFriction;
		}
		else if (FloorAngleDeg > -1.f && FloorAngleDeg < SlideAngleDeg && VelocityX < 0)
		{
			// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Floor Angle = %f"), FloorAngleDeg);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide to false"));
			UE_LOG(LogTemp, Verbose,
				TEXT("APC_PlayerFox::CrouchClimbDown - LEFT - setting bIsSliding to false.")
			);
			this->bIsSliding = false;
			this->GetCharacterMovement()->BrakingFrictionFactor = this->StandingFriction;
		}
	}
	/////////////////////////

	if (bOverlappingClimbable && !this->bIsSliding)
	{
		Climb(-ClimbSpeed);
	} else {
		this->bIsCrouching = true;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(
			FVector(
				this->GetProjectileSpawnPoint()->GetRelativeLocation().X,
				0.f,
				ProjectileSpawnLoc.Z - CrouchProjectileSpawnPoint.Z
			)
		);
		
		// UE_LOG(
		// 	LogTemp, VeryVerbose, TEXT("%s's location is %s, and projectile spawn point is %s!"),
		// 	*this->GetName(),
		// 	*this->GetActorLocation().ToString(),
		// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
		// );
	}
}

/**
 * Climb up the ladder if the player is overlapping a climbable object.
 * If the player is not overlapping a climbable object, shoot upward by setting up necessary variables.
 *
 * @param None.
 * @return None.
 */
void APC_PlayerFox::ClimbUp()
{
	if (bOverlappingClimbable)
	{
		this->bOnLadder = true;
		Climb(ClimbSpeed);
	}
	else
	{
		// const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
		this->bIsCrouching = false;
		this->ShootUpward = true;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(ProjectileUpwardSpawnLoc);
		
		// UE_LOG(
		// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
		// 	*this->GetName(),
		// 	*this->GetActorLocation().ToString(),
		// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
		// );
	}
}

/**
 * Stops the crouch climb action if the character is currently crouching.
 *
 * @param void
 * @return void
 */
void APC_PlayerFox::StopCrouchClimb()
{
	if (this->bIsCrouching)
	{
		const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
		this->bIsSliding = false;
		this->GetCharacterMovement()->BrakingFrictionFactor = this->StandingFriction;
		this->bIsCrouching = false;
		this->GetProjectileSpawnPoint()->SetRelativeLocation(
			FVector(ProjSpawnLoc.X, ProjectileSpawnLoc.Y, ProjSpawnLoc.Z)
		);
		
		// UE_LOG(
		// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
		// 	*this->GetName(),
		// 	*this->GetActorLocation().ToString(),
		// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
		// );
	}

	// set ShootUpward to false if its true
	this->ShootUpward &= false;
	
	const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
	this->GetProjectileSpawnPoint()->SetRelativeLocation(
		FVector(ProjSpawnLoc.X, ProjectileSpawnLoc.Y, ProjectileSpawnLoc.Z)
	);
	
	// UE_LOG(
	// 	LogTemp, Warning, TEXT("%s's location is %s, and projectile spawn point is %s!"),
	// 	*this->GetName(),
	// 	*this->GetActorLocation().ToString(),
	// 	*this->GetProjectileSpawnPoint()->GetRelativeLocation().ToString()
	// );
	
	this->StopClimb();
}

/**
 * Stops the climb action of the fox player.
 *
 * @param None
 *
 * @return None
 */
void APC_PlayerFox::StopClimb()
{
	if (this->bOnLadder && this->GetVelocity().Z != 0)
	{
		this->GetMovementComponent()->StopMovementImmediately();
	}
	this->bIsClimbing = false;
}

TArray<APC_PlayerFox*> APC_PlayerFox::GetSpectators() const
{
	
	return this->Spectators;
}

/**
 * SetSpectatorsStr method sets the SpectatorsStr member variable of the APC_PlayerFox class.
 * The SpectatorsStr is a string that represents all the spectators of the APC_PlayerFox instance.
 * This method creates the SpectatorsStr by looping through each spectator and concatenating their names.
 *
 * @param None
 * @return None
 */
void APC_PlayerFox::SetSpectatorsStr()
{
	FString SpectatorsString = "";
	for (const APC_PlayerFox* Spectator : this->Spectators)
	{
		UE_LOG(
			LogTemp, Display, TEXT("FOR LOOP: one of %s's spectators is: %s"),
			*this->GetPlayerName().ToString(),
			*Spectator->GetPlayerName().ToString()
		);
		SpectatorsString += Spectator->GetPlayerName().ToString() + "\n";
		UE_LOG(
			LogTemp, Display, TEXT("FOR LOOP: %s's spectators string is %s"),
			*this->GetPlayerName().ToString(),
			*SpectatorsString
		);
	}
	UE_LOG(
		LogTemp, Display, TEXT("AFTER FOR LOOP: %s's complete spectator string is %s"),
		*this->GetPlayerName().ToString(),
		*SpectatorsString
	);
	this->SpectatorsStr = SpectatorsString;
}

/**
 * Returns the spectator string representation.
 *
 * @return The spectator string representation as FText.
 */
FText APC_PlayerFox::GetSpectatorsAsStr() const
{
	// UE_LOG(
	// 	LogTemp, Display, TEXT("%s's spectator string is: %s"),
	// 	*this->GetName(),
	// 	*this->SpectatorsStr
	// );
	return FText::FromString(this->SpectatorsStr);
}

/**
 * Retrieves the name of the player.
 *
 * @return The name of the player.
 */
FText APC_PlayerFox::GetPlayerName() const
{
	return FText::FromString(this->PlayerName);
}

/**
 * Add a player to the list of spectators.
 *
 * This method adds the specified player to the list of spectators for this instance.
 * After adding the player, the SetSpectatorsStr() method is called to update the spectators string.
 *
 * @param Spectator The player to be added as a spectator.
 *
 * @see SetSpectatorsStr
 */
void APC_PlayerFox::AddToSpectators(APC_PlayerFox* Spectator)
{
	this->Spectators.Add(Spectator);
	this->SetSpectatorsStr();
}

/**
 * @brief Removes a player from the spectators list.
 *
 * This method removes the given player from the spectators list of the calling player.
 *
 * @param Spectator The player to be removed from the spectators list.
 */
void APC_PlayerFox::RemoveFromSpectators(APC_PlayerFox* const Spectator)
{
	if (this->Spectators.Contains(Spectator))
	{
		this->Spectators.Remove(Spectator);
		this->SetSpectatorsStr();
	}
}

/**
 * ClimbUpAxisInputCallback
 *
 * @param Z The input value along the Z-axis
 *
 * This method is called when the user provides input for climbing up or down. It handles the logic for climbing up,
 * crouch climbing down, and stopping the crouch climb.
 * If the input value is greater than 0, it calls the ClimbUp method.
 * If the input value is less than 0, it calls the CrouchClimbDown method.
 * If the input value is 0, it calls the StopCrouchClimb method.
 *
 * Additionally, if the local role of the player is ROLE_AutonomousProxy, it sends the vertical axis input value
 * to the server using the SendVerticalAxisInputToServer method.
 */
void APC_PlayerFox::ClimbUpAxisInputCallback(const float Z)
{
	if (Z > 0) {
		ClimbUp();
	} else if (Z < 0) {
		CrouchClimbDown();
	} else {
		StopCrouchClimb();
	}

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		SendVerticalAxisInputToServer(Z);
	}
}

/**
 * Move the player character horizontally to the right based on the input axis.
 *
 * @param Axis The input axis value for moving right. Positive value indicates moving right and negative value indicates
 * moving left.
 *
 * @note This method should not be called if the player character is in the hurt animation or is crouching.
 *
 * @details
 * This method updates the rotation of the player character based on the input axis.
 * If the player character is on a ladder, it moves horizontally at a reduced speed compared to normal movement.
 * If the player character is not on a ladder, it moves horizontally at the normal speed.
 *
 * If the player character is not overlapping with a climbable object and is not falling,
 * the movement mode is set to walking.
 *
 * @see APC_PlayerFox::UpdateRotation
 * @see APC_PlayerFox::GetSprite
 * @see APC_PlayerFox::GetCharacterMovement
 * @see APC_PlayerFox::GetMovementComponent
 * @see APC_PlayerFox::AddMovementInput
 * @see APC_PlayerFox::bOverlappingClimbable
 * @see APC_PlayerFox::bIsFalling
 * @see ECharacterMovementType
 **/
void APC_PlayerFox::MoveRight(const float Axis)
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	// early return if player is crouching right now
	if (this->bIsCrouching || this->bIsClimbing) {return;}
	
	UpdateRotation(Axis);

	// on ladder == flying
	if (GetCharacterMovement()->GetMovementName() == "Flying")
	{
		if (Axis == 0)  // if not pressing move left or right button
		{
			GetMovementComponent()->StopMovementImmediately();
		}
		else
		{
			// move horizontally on ladder at a reduced speed compared to normal
			AddMovementInput(FVector(Axis * ClimbingLateralSpeed, 0, 0));
		}
	}
	else
	{
		// move left/right normally
		AddMovementInput(FVector(Axis, 0, 0));
	}


	if (!bOverlappingClimbable && !bIsFalling)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

/**
 * Climb the object.
 *
 * @param Value The value to control the climbing movement.
 */
void APC_PlayerFox::Climb(const float Value)
{
	if (!this->bIsClimbing){
		GetMovementComponent()->StopMovementImmediately();
		this->bIsClimbing = true;
	}
	this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	AddMovementInput(GetActorUpVector(), Value);
}

/**
 * @brief Function to make the player character jump.
 *
 * This function is responsible for making the player character jump. It checks
 * if the character is currently in a hurt animation, and if so, it does an
 * early return and doesn't allow the jump. If the character is not currently
 * jumping or on a ladder, it plays a jump sound and calls the parent class's
 * jump function to perform the actual jump.
 *
 * @param None
 * @return None
 */
void APC_PlayerFox::Jump()
{
	// early return if player in hurt animation right now
	if (this->GetSprite()->GetFlipbook() == HurtAnimation) {return;}
	
	// dont allow another jump unless not currently jumping
	if (!this->bIsFalling && !this->bOnLadder) {
		UGameplayStatics::SpawnSoundAttached(
			this->JumpSound,
			this->GetSprite(),
			TEXT("PaperCharacterSpriteJump")
		);
		Super::Jump();
	}
}

/**
 * Increases the player's money stash by a given monetary value.
 *
 * @param MonetaryValue The amount of money to be added to the money stash.
 */
void APC_PlayerFox::TakeMoney(int MonetaryValue)
{
	this->MoneyStash += MonetaryValue;
	UE_LOG(LogTemp, Verbose,
		TEXT("%s's money stash is now %i!"), *this->GetName(), this->MoneyStash
	);
}

/**
 * @brief Takes healing and increases the player's health.
 *
 * This function takes a healing value as a parameter and increases the player's health by that value.
 *
 * @param HealingValue   The amount of healing to add to the player's health.
 *
 * @see APC_PlayerFox::AddHealth
 * @see APC_PlayerFox::GetName
 * @see APC_PlayerFox::GetHealth
 * @see UE_LOG
 *
 * @note This function does not return anything.
 * @note This function assumes that the player's health is a float value.
 * @note This function assumes that the player's health can be accessed and modified through the AddHealth() and
 * GetHealth() functions respectively.
 * @note This function assumes that the player's name can be accessed through the GetName() function.
 * @note This function assumes that the Unreal Engine logging system is available and can be used with UE_LOG.
 * @note This function is a member of the APC_PlayerFox class.
 */
void APC_PlayerFox::TakeHealing(const float HealingValue)
{
	this->AddHealth(HealingValue);
	UE_LOG(LogTemp, Verbose,
		TEXT("%s's health is now %f!"), *this->GetName(), this->GetHealth()
	);
}

/**
 * Adds the specified number of cherries to the player's cherry stash.
 *
 * @param NumCherries The number of cherries to add to the stash.
 */
void APC_PlayerFox::TakeCherries(int NumCherries)
{
	this->CherryStash += NumCherries;
	UE_LOG(LogTemp, Verbose,
		TEXT("%s's cherry stash has increased to %d!"),
		*this->GetName(),
		this->CherryStash
	);
}

void APC_PlayerFox::LogSpeed()
{
	UE_LOG(LogTemp, VeryVerbose,
		TEXT("%s's speed is %f!"), *this->GetName(), this->GetVelocity().Size()
	);
}

void APC_PlayerFox::LogRotation()
{
	UE_LOG(LogTemp, VeryVerbose,
		TEXT("%s's rotation is %s!"), *this->GetName(), *GetSprite()->GetRelativeRotation().ToString()
	);
}

/**
 * @brief Sets the run velocity of the Fox player character.
 *
 * This method sets the maximum walk speed of the Fox player character to the specified running speed.
 * If the local role of the character is ROLE_AutonomousProxy (controller on client), it also calls the
 * SetRunVelocityRPC() method which is ran on the server.
 *
 * @param None
 * @return None
 */
void APC_PlayerFox::SetRunVelocity()
{
	this->GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		SetRunVelocityRPC();	
	}
}

/**
 * @brief Implementation of the SetRunVelocityRPC method.
 *
 * This method sets the maximum running speed for the character's movement.
 *
 * @param MaxRunningSpeed The new maximum running speed for the character.
 *
 * @details This method sets the MaxWalkSpeed property of the character's movement component
 *          to the specified MaxRunningSpeed value.
 *
 * @note This method is called remotely and implemented on the server side.
 *
 * @see GetCharacterMovement(), MaxWalkSpeed
 */
void APC_PlayerFox::SetRunVelocityRPC_Implementation()
{
	this->GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
}

bool APC_PlayerFox::SetRunVelocityRPC_Validate()
{
	return true;
}

/**
 * @brief Sets the walk velocity of the player's character.
 *
 * This method sets the maximum walk speed of the player's character to the value specified by the NormalWalkingSpeed
 * variable.
 * If the player's role is autonomous proxy (controller on client), it also calls the SetWalkVelocityRPC method which
 * is ran on the server.
 *
 * @note This method should be called whenever the walk velocity of the character needs to be changed.
 *
 * @see SetWalkVelocityRPC()
 *
 * @param None.
 * @return None.
 */
void APC_PlayerFox::SetWalkVelocity()
{
	this->GetCharacterMovement()->MaxWalkSpeed = NormalWalkingSpeed;
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		SetWalkVelocityRPC();	
	}
}

/**
 * @brief Sets the walk velocity of the player fox's character movement.
 *
 * This method is called to set the max walk speed of the character movement
 * component of the player fox. It is an implementation of the RPC (Remote Procedure Call)
 * function.
 *
 * @param None.
 * @return None.
 */
void APC_PlayerFox::SetWalkVelocityRPC_Implementation()
{
	this->GetCharacterMovement()->MaxWalkSpeed = NormalWalkingSpeed;
}

bool APC_PlayerFox::SetWalkVelocityRPC_Validate()
{
	return true;
}

void APC_PlayerFox::LogLocation()
{
	UE_LOG(LogTemp, VeryVerbose,
		TEXT("%s's location is %s!"), *this->GetName(), *this->GetActorLocation().ToString()
	);
}

/**
 * Opens the menu based on the game mode.
 *
 * This method is responsible for opening the menu based on the current game mode. If the game mode is a level game mode,
 * the in-game menu will be opened. If the game mode is a lobby game mode, the character selection menu will be opened.
 */
void APC_PlayerFox::OpenMenu()
{
	const ALevelGameMode* LevelGameMode = dynamic_cast<ALevelGameMode*>(GetWorld()->GetAuthGameMode());
	if (LevelGameMode != nullptr)
	{
		OpenInGameMenu();
		return;
	}

	// TODO: call this in a separate function on from the server to the client
	ALobbyGameState* GameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (GameState != nullptr)
	{
		GameState->OpenSelectCharacterMenu();
	}
}

/**
 * Opens the in-game menu.
 *
 * This method is responsible for opening the in-game menu. It pauses the game
 * and calls the `InGameLoadMenu` method of the game instance.
 *
 * @param None
 *
 * @return None
 */
void APC_PlayerFox::OpenInGameMenu()
{
	if (GameInstance != nullptr) {
		// TODO: remove pause and make this function call on server
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GameInstance->InGameLoadMenu();
	}
	else {
		UE_LOG(LogTemp, Warning,
			TEXT("APC_PlayerFox::OpenInGameMenu - Can't open InGameMenu. GameInstance is null!")
		);
	}
}

/**
 * @brief Sets the flag indicating whether the player can shoot again.
 *
 * This method sets the value of the member variable `bCanShoot` to `true`.
 *
 * @param None.
 * @return None.
 */
void APC_PlayerFox::CanShootAgain()
{
	this->bCanShoot = true;
}

/** Shoots a projectile if the player is allowed to shoot.
 *
 * If the player's ability to shoot is set to true (bCanShoot), this method will perform the following actions:
 *  - Call the Super class's Shoot method
 *  - Set bCanShoot to false to prevent shooting again until delay time has elapsed
 *  - Start a timer that will call the CanShootAgain method after the specified delay time
 *
 * @param None
 * @return None
 */
void APC_PlayerFox::Shoot()
{
	if (this->bCanShoot)
	{
		Super::Shoot();
		this->bCanShoot = false;

		GetWorld()->GetTimerManager().SetTimer(
			this->ShootTimerHandle,
			this,
			&APC_PlayerFox::CanShootAgain,
			this->GetShootDelayTime(),
			false
		);
	}
}
