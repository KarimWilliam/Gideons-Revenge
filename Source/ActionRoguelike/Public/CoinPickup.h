// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "CoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ACoinPickup : public APickupBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACoinPickup();

protected:
	FTimerHandle HiddenTimerHandler;
	void FHiddenTimer();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
