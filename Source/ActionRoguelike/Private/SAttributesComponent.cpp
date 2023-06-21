// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	Health=100;
	MaxHealth=100.0f;
	
	
}


void USAttributesComponent::SetMaxHealth(const float MaxH)
{
	this->MaxHealth = MaxH;
}

float USAttributesComponent::GetMaxHealth()
{
	return MaxHealth;
}

// Called when the game starts
void USAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

bool USAttributesComponent::IsAlive () const
{
	return Health>0.f;
}


// Called every frame
void USAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USAttributesComponent::ApplyHealthChange(float Delta)
{
	float OldHealth=Health;
	Health = FMath::Clamp(Health+Delta, 0.0f, MaxHealth); //What if maxhealth is changed later on?
	//call the custom event we made.
	float ActualDelta= Health-OldHealth;
	OnHealthChanged.Broadcast(nullptr,this,Health,ActualDelta);
	return ActualDelta!=0;
}

