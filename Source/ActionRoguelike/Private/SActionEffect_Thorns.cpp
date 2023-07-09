// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "GamePlayFunctionLibrary.h"
#include "SActionComponent.h"
#include "SAttributesComponent.h"


USActionEffect_Thorns::USActionEffect_Thorns()
{

	ReflectFraction=0.2f;
	Duration=0;
	Period=0;
}


void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator) //when this effect is added to my character, i will bind a new function to the attribute's on health changed function to add extra effects.
//it is a bad idea to add such functionalities to the Attribute's OnHealthChange function because it will add a lot of complexity over time in the wrong place. 
{
	Super::StartAction_Implementation(Instigator);

	if(USAttributesComponent* Attributes= USAttributesComponent::GetAttributes(GetOwningComponent()->GetOwner()))
	{
		Attributes->OnHealthChanged.AddDynamic(this,&USActionEffect_Thorns::OnHealthChanged); // OnHealthChanged is a custom event. this is how to bind/subscribe to a custom event
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening. Remove the binding.
	USAttributesComponent* Attributes = USAttributesComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}



void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0.0f && OwningComp->GetOwner()!=InstigatorActor)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

	UGamePlayFunctionLibrary::ApplyDamage(OwningComp->GetOwner(),InstigatorActor,ReflectedAmount);
		
	}

}
