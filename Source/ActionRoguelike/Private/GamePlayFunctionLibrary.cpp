// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayFunctionLibrary.h"
#include "SAttributesComponent.h"

bool UGamePlayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}


	return false;
}

bool UGamePlayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
                                                      const FHitResult& HitResult)
{
	USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(TargetActor);
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp= HitResult.GetComponent();
		if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal*30000.f,HitResult.ImpactPoint,HitResult.BoneName);
		}
		return true;
	}
	return false;
}
