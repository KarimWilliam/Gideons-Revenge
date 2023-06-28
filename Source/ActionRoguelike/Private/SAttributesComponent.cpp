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

float USAttributesComponent::GetHealth()
{
	return Health;
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

bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	float OldHealth=Health;
	Health = FMath::Clamp(Health+Delta, 0.0f, MaxHealth); //What if maxhealth is changed later on?
	//call the custom event we made.
	float ActualDelta= Health-OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor,this,Health,ActualDelta);
	DrawDebugString(GetWorld(),GetOwner()->GetActorLocation(),FString::SanitizeFloat(Health),nullptr,FColor::Red,4.f,true);
	return ActualDelta!=0;
}

bool USAttributesComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-GetMaxHealth());
}

USAttributesComponent* USAttributesComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<USAttributesComponent>(FromActor->GetComponentByClass(USAttributesComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributesComponent::IsActorAlive(AActor* Actor)
{
	USAttributesComponent* AttributesComp=GetAttributes(Actor);
	if(AttributesComp)
	{
		return AttributesComp->IsAlive();
	}
	return false;
}

