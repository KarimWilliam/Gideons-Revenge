// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotionPickup.h"
#include "SAttributesComponent.h"
#include "SCharacter.h"


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
}

void AHealthPotionPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	USAttributesComponent* AttributeComp= Cast<USAttributesComponent> (InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass()));

	if(AttributeComp->GetMaxHealth()!=AttributeComp->Health)
	{
		AttributeComp->ApplyHealthChange(20.0f);
		SetActorHiddenInGame(true);

		GetWorldTimerManager().SetTimer(HiddenTimerHandler,this,&AHealthPotionPickup::FHiddenTimer, 10.f);
	}


}

