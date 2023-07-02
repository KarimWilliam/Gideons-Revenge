// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}
	USAction* NewAction= NewObject<USAction>(this,ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
		
	}
	return false;
}
