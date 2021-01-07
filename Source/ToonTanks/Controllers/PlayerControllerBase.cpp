#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnabledState(bool PlayerEnabled) {
	if (!GetPawn()) {
		UE_LOG(LogTemp, Error, TEXT("Cannot get Pawn"));
		return;
	}
	
	if (PlayerEnabled) {
		GetPawn()->EnableInput(this);
	} else {
		GetPawn()->DisableInput(this);
	}

	bShowMouseCursor = PlayerEnabled;
}