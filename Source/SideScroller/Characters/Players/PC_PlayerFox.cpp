#include "PC_PlayerFox.h"

#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/GameModes/LevelGameMode.h"
#include "SideScroller/GameModes/SideScrollerGameModeBase.h"
#include "SideScroller/GameStates/LobbyGameState.h"
#include "SideScroller/SaveGames/SideScrollerSaveGame.h"

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

	DoLevelWelcome();
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

void APC_PlayerFox::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APC_PlayerFox, SpectatorsStr);
	DOREPLIFETIME(APC_PlayerFox, PlayerBeingSpectated);
	DOREPLIFETIME(APC_PlayerFox, Spectators);
	DOREPLIFETIME(APC_PlayerFox, MaxRunningSpeed);
	DOREPLIFETIME(APC_PlayerFox, bIsClimbing);
	DOREPLIFETIME(APC_PlayerFox, bIsCrouching);
	DOREPLIFETIME(APC_PlayerFox, bIsSliding);
	DOREPLIFETIME(APC_PlayerFox, bOnLadder);
	DOREPLIFETIME(APC_PlayerFox, CurrentRotation);
	DOREPLIFETIME(APC_PlayerFox, PlayerName);
	DOREPLIFETIME(APC_PlayerFox, WalkSound);
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

void APC_PlayerFox::PrintPlayersList(TArray<APC_PlayerFox*> PlayersArray)
{
	FString PlayerArrayStr = "";
	for (const APC_PlayerFox* Player : PlayersArray)
	{
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("APC_PlayerFox::PrintPlayersList - Found null Player."));
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

void APC_PlayerFox::DoLevelComplete()
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

bool APC_PlayerFox::FoundPlayerToSpectate(APC_PlayerFox* Player)
{
	if (Player != nullptr && Player != this && !Player->IsDead()) {
		if (this->PlayerBeingSpectated != nullptr)
		{
			UE_LOG(LogTemp, Display,
				TEXT("Removing %s from %s's spectator list."),
				*this->GetName(),
				*this->PlayerBeingSpectated->GetName()
			);
			this->PlayerBeingSpectated->RemoveFromSpectators(this);
		}
		this->PlayerBeingSpectated = Player;
		
		UE_LOG(LogTemp, Display,
			TEXT("Adding %s to %s's spectator list."),
			*this->GetName(),
			*this->PlayerBeingSpectated->GetName()
		);

		this->PlayerBeingSpectated->AddToSpectators(this);
		Spectate();
		return true;
	}
	return false;
}

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

void APC_PlayerFox::SpectateOtherPlayer()
{
	if (const ASideScrollerGameModeBase* GameMode = dynamic_cast<ASideScrollerGameModeBase*>(
		GetWorld()->GetAuthGameMode()
		)
	) {
		BeginSpectating(GameMode);
	}
}

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

void APC_PlayerFox::UseAction()
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

void APC_PlayerFox::MoveSpectatorsToNewPlayer() const
{
	TArray<APC_PlayerFox*> PlayerSpectators = this->GetSpectators();
	for (APC_PlayerFox* PlayerSpectator : PlayerSpectators)
	{
		PlayerSpectator->SpectateNextPlayer();
	}
}

void APC_PlayerFox::ReviveAtCheckpoint()
{
	// set location back to last checkpoint
	this->SetHealth(this->DefaultHealth);
	this->SetActorLocation(
		LastCheckpointLocation, false, nullptr, ETeleportType::ResetPhysics
	);
	this->GetMovementComponent()->StopMovementImmediately();
}

void APC_PlayerFox::PlayerDeath()
{
	if (this->NumberOfLives > 0)
	{
		// take a life away
		this->NumberOfLives -= 1;
		
		ReviveAtCheckpoint();
	} else {
		this->RemoveFromPlayersArray();
		this->DoDeath();

		this->bIsOutOfLives = true;
		SpectateNextPlayer();  // SpectateOtherPlayer();
	}
}

void APC_PlayerFox::HandleFallOffLevel()
{
	// Maybe one color character doesnt die when it falls off, wrap PlayerDeath in that case.
	PlayerDeath();
}

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
		UE_LOG(LogTemp, Warning, TEXT("APC_PlayerFox::PlayerHUDTeardown - WidgetPlayerHUDInstance is null"))
	}
}

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

void APC_PlayerFox::DeathCleanUp()
{
	this->RemoveFromPlayersArray();
	this->PlayerHUDTeardown();
	this->PlayerMessageWidgetTeardown();
	this->MoveSpectatorsToNewPlayer();
}

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

void APC_PlayerFox::UpdateRotation(const float Value)
{
	const FVector ProjSpawnLoc = GetProjectileSpawnPoint()->GetRelativeLocation();
	
	if (Value < 0) {
		this->CurrentRotation = MovingLeftRotation;
		this->GetSprite()->SetRelativeRotation(MovingLeftRotation);
		GetProjectileSpawnPoint()->SetRelativeLocation(FVector(ProjectileSpawnLoc.X * -1.f, ProjSpawnLoc.Y, ProjSpawnLoc.Z));
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

void APC_PlayerFox::UpdateNameBanner()
{
	if (NameBanner->Text.EqualTo(FText::FromString("")))
	{
		LoadProfilePlayerName();
		NameBanner->SetText(FText::FromString(this->PlayerName));
	}
}

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

void APC_PlayerFox::CrouchClimbDown()
{
	// Run slide ////////////
	const float VelocityX = this->GetVelocity().X;
	if (!this->bIsSliding && (abs(VelocityX) > this->CrouchSlidingThresholdVelocity))
	{
		// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
		// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide (run) to true"));
		UE_LOG(LogTemp, Display, TEXT("APC_PlayerFox::CrouchClimbDown - setting bIsSliding to true for run."));
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
			UE_LOG(LogTemp, Verbose, TEXT("APC_PlayerFox::CrouchClimbDown - RIGHT - setting bIsSliding true - hill."));
			this->bIsSliding = true;
			this->GetCharacterMovement()->BrakingFrictionFactor = 0.f;  // this->CrouchSlideFriction;
			return;
		}

		if ((FloorAngleDeg >= SlideAngleDeg) && VelocityX < 0)
		{
			// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Floor Angle = %f"), FloorAngleDeg);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide (hill) to true"));
			UE_LOG(LogTemp, Verbose, TEXT("APC_PlayerFox::CrouchClimbDown - LEFT - setting bIsSliding true - hill."));
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
			UE_LOG(LogTemp, Verbose, TEXT("APC_PlayerFox::CrouchClimbDown - RIGHT - setting bIsSliding to false."));
			this->bIsSliding = false;
			this->GetCharacterMovement()->BrakingFrictionFactor = this->StandingFriction;
		}
		else if (FloorAngleDeg > -1.f && FloorAngleDeg < SlideAngleDeg && VelocityX < 0)
		{
			// UE_LOG(LogTemp, VeryVerbose, TEXT("X Vel = %f"), VelocityX);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("Floor Angle = %f"), FloorAngleDeg);
			// UE_LOG(LogTemp, VeryVerbose, TEXT("APC_PlayerFox::CrouchClimbDown - setting slide to false"));
			UE_LOG(LogTemp, Verbose, TEXT("APC_PlayerFox::CrouchClimbDown - LEFT - setting bIsSliding to false."));
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

void APC_PlayerFox::SetSpectatorsStr()
{
	FString SpectatorsString = "";
	for (const APC_PlayerFox* Spectator : this->Spectators)
	{
		UE_LOG(
			LogTemp, Display, TEXT("FOR LOOP: one of %s's spectators is: %s"),
			*this->GetName(),
			*Spectator->GetName()
		);
		SpectatorsString += Spectator->GetName() + "\n";
		UE_LOG(
			LogTemp, Display, TEXT("FOR LOOP: %s's spectators string is %s"),
			*this->GetName(),
			*SpectatorsString
		);
	}
	UE_LOG(
		LogTemp, Display, TEXT("AFTER FOR LOOP: %s's complete spectator string is %s"),
		*this->GetName(),
		*SpectatorsString
	);
	this->SpectatorsStr = SpectatorsString;
}

FText APC_PlayerFox::GetSpectatorsAsStr() const
{
	// UE_LOG(
	// 	LogTemp, Display, TEXT("%s's spectator string is: %s"),
	// 	*this->GetName(),
	// 	*this->SpectatorsStr
	// );
	return FText::FromString(this->SpectatorsStr);
}

FText APC_PlayerFox::GetPlayerName() const
{
	return FText::FromString(this->PlayerName);
}

void APC_PlayerFox::AddToSpectators(APC_PlayerFox* Spectator)
{
	this->Spectators.Add(Spectator);
	this->SetSpectatorsStr();
}

void APC_PlayerFox::RemoveFromSpectators(APC_PlayerFox* const Spectator)
{
	if (this->Spectators.Contains(Spectator))
	{
		this->Spectators.Remove(Spectator);
		this->SetSpectatorsStr();
	}
}

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

void APC_PlayerFox::Climb(const float Value)
{
	if (!this->bIsClimbing){
		GetMovementComponent()->StopMovementImmediately();
		this->bIsClimbing = true;
	}
	this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	AddMovementInput(GetActorUpVector(), Value);
}

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

void APC_PlayerFox::TakeMoney(int MonetaryValue)
{
	this->MoneyStash += MonetaryValue;
	UE_LOG(LogTemp, Verbose, TEXT("%s's money stash is now %i!"), *this->GetName(), this->MoneyStash);
}

void APC_PlayerFox::TakeHealing(const float HealingValue)
{
	this->AddHealth(HealingValue);
	UE_LOG(LogTemp, Verbose, TEXT("%s's health is now %f!"), *this->GetName(), this->GetHealth());
}

void APC_PlayerFox::TakeCherries(int NumCherries)
{
	this->CherryStash += NumCherries;
	UE_LOG(LogTemp, Verbose, TEXT("%s's cherry stash has increased to %d!"), *this->GetName(), this->CherryStash);
}

void APC_PlayerFox::LogSpeed()
{
	UE_LOG(LogTemp, VeryVerbose, TEXT("%s's speed is %f!"), *this->GetName(), this->GetVelocity().Size());
}

void APC_PlayerFox::LogRotation()
{
	UE_LOG(LogTemp, VeryVerbose,
		TEXT("%s's rotation is %s!"), *this->GetName(), *GetSprite()->GetRelativeRotation().ToString()
	);
}

void APC_PlayerFox::SetRunVelocity()
{
	this->GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
}

void APC_PlayerFox::SetWalkVelocity()
{
	this->GetCharacterMovement()->MaxWalkSpeed = NormalWalkingSpeed;
}

void APC_PlayerFox::LogLocation()
{
	UE_LOG(LogTemp, VeryVerbose, TEXT("%s's location is %s!"), *this->GetName(), *this->GetActorLocation().ToString());
}

void APC_PlayerFox::OpenMenu()
{
	const ALevelGameMode* LevelGameMode = dynamic_cast<ALevelGameMode*>(GetWorld()->GetAuthGameMode());
	if (LevelGameMode != nullptr)
	{
		OpenInGameMenu();
		return;
	}
	
	ALobbyGameState* GameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (GameState != nullptr)
	{
		GameState->OpenSelectCharacterMenu();
	}
}

void APC_PlayerFox::OpenInGameMenu()
{
	if (GameInstance != nullptr) {
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GameInstance->InGameLoadMenu();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APC_PlayerFox::OpenInGameMenu - Can't open InGameMenu. GameInstance is null!"));
	}
}

void APC_PlayerFox::CanShootAgain()
{
	this->bCanShoot = true;
}

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
