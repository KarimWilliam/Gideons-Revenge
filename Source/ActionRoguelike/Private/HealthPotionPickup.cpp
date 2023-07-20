// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotionPickup.h"
#include "SAttributesComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"


// Sets default values
AHealthPotionPickup::AHealthPotionPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHealthPotionPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHealthPotionPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AHealthPotionPickup::FHiddenTimer()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AHealthPotionPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* PC = Cast<ASCharacter>(InstigatorPawn))
	{
		if (ASPlayerState* PlayerState = Cast<ASPlayerState>(PC->GetPlayerState()))
		{
			USAttributesComponent* AttributeComp = Cast<USAttributesComponent>(
				InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass()));

			if (AttributeComp->GetMaxHealth() != AttributeComp->Health)
			{
				if (PlayerState->ApplyCreditChange(-100))
				//if player has enough credits he is allowed to pick up the health potiion
				{
					AttributeComp->ApplyHealthChange(this, 20.0f);
					SetActorHiddenInGame(true);
					SetActorEnableCollision(false);

					GetWorldTimerManager().SetTimer(HiddenTimerHandler, this, &AHealthPotionPickup::FHiddenTimer, 10.f);
				}
			}
		}
	}
}

FText AHealthPotionPickup::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributesComponent* AttributeComp= USAttributesComponent::GetAttributes(InstigatorPawn);
	if(AttributeComp && AttributeComp->GetHealth()==AttributeComp->GetMaxHealth())
	{
		return NSLOCTEXT("InteractableActors","HealthPotion_FullHealthWarning","Already at full health");
	}
	return NSLOCTEXT("InteractableActors","HealthPotion_FInteractMessage","Cost 200 Credits. Restores 50 health"); 
}
