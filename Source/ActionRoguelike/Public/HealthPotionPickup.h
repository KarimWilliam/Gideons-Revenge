// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "HealthPotionPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AHealthPotionPickup : public APickupBase
{
	GENERATED_BODY()

	//void Interact_Implementation(APawn* InstigatorPawn);

public:
	// Sets default values for this actor's properties
	AHealthPotionPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle HiddenTimerHandler;
	void FHiddenTimer();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
