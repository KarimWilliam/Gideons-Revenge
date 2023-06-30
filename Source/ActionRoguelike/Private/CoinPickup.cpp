// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinPickup.h"

#include "SCharacter.h"
#include "SPlayerState.h"

ACoinPickup::ACoinPickup()
{
}

void ACoinPickup::FHiddenTimer()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ACoinPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* PC = Cast<ASCharacter>(InstigatorPawn))
	{
		if (ASPlayerState* PlayerState = Cast<ASPlayerState>(PC->GetPlayerState()))
		{
			PlayerState->ApplyCreditChange(200);
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);

			GetWorldTimerManager().SetTimer(HiddenTimerHandler, this, &ACoinPickup::FHiddenTimer, 10.f);
		}
	}
}
