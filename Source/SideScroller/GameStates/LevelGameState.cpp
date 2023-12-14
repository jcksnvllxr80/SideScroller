#include "LevelGameState.h"

#include "SideScroller/SideScrollerGameInstance.h"

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
