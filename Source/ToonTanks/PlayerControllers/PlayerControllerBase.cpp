// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

void APlayerControllerBase::SetPlayerEnableState(bool bSetPlayerEnabled)
{
	APawn* PlayerPawn = GetPawn();
	if (!PlayerPawn)
	{
		return;
	}

	if (bSetPlayerEnabled )
	{
		PlayerPawn->EnableInput(this);
	}
	else
	{
		PlayerPawn->DisableInput(this);
	}

	bShowMouseCursor = bSetPlayerEnabled;
}