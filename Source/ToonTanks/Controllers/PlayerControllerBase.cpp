#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnabledState(bool PlayerEnabled) {
	if (PlayerEnabled) {
		GetPawn()->EnableInput(this);
	} else {
		GetPawn()->DisableInput(this);
	}

	bShowMouseCursor = PlayerEnabled;
}