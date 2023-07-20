// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running %s"),*GetNameSafe(this))

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGamePlayTags.AppendTags(GrantsTags);

	bIsRunning=true;

	TimeStarted=GetWorld()->TimeSeconds;

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(),this);
	
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running %s"),*GetNameSafe(this))

	ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	Comp-> ActiveGamePlayTags.RemoveTags(GrantsTags);

	bIsRunning=false;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(),this);
}


UWorld* USAction::GetWorld() const
{
	UActorComponent* Comp=Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}


USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}



bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}
	USActionComponent* Comp = GetOwningComponent();
	if(Comp->ActiveGamePlayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

