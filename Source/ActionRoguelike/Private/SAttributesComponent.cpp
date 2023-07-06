// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"),1.0f,TEXT("Gloal Damage modifier for Attribute component"),ECVF_Cheat);

// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{
	MaxHealth=100.0f;
	Health=MaxHealth;
	
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
if(Delta<0)
{
	Delta*=CVarDamageMultiplier.GetValueOnGameThread();
}

	
	float OldHealth=Health;
	Health = FMath::Clamp(Health+Delta, 0.0f, MaxHealth); //What if maxhealth is changed later on?
	//call the custom event we made.
	float ActualDelta= Health-OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor,this,Health,ActualDelta);
	//DrawDebugString(GetWorld(),GetOwner()->GetActorLocation(),FString::SanitizeFloat(Health),nullptr,FColor::Red,4.f,true);

	//Died
	if(ActualDelta<0.0f && Health==0.0f)
	{
		ASGameModeBase* GM= GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(),InstigatorActor);
		}
	}
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

