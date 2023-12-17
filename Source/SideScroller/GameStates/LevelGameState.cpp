#include "LevelGameState.h"

#include "SideScroller/SideScrollerGameInstance.h"

/**
 * Opens the respawn menu.
 *
 * This method is responsible for opening the respawn menu in the game. It first checks if the game instance
 * is of type USideScrollerGameInstance by dynamically casting it. If the cast is successful, it calls the
 * RespawnLoadMenu() method on the game instance. If the cast fails, it logs a warning message indicating
 * that the game instance could not be found.
 *
 * @param None
 * @return None
 */
void ALevelGameState::OpenRespawnMenu()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance != nullptr) {
		GameInstance->RespawnLoadMenu();
	} else {
		UE_LOG(LogTemp, Warning,
			TEXT("ASideScrollerGameState::OpenRespawnMenu - Cant find GameInstance!")
		);
	}
}
