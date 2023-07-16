// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	Credits=0;
	SetReplicates(true);
}

int ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::SetCredits(int creds)
{
	this->Credits = creds;
}

bool ASPlayerState::ApplyCreditChange(const int creds)
{
	if(creds<0 && (creds*-1>this->Credits))// cant spend more than what you have.
	{
		return false;
	}
	this->Credits+=creds;
	return true;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState,Credits);

	//DOREPLIFETIME_CONDITION(USAttributesComponent,MaxHealth,COND_OwnerOnly);
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{

	if(SaveObject)
	{
		SaveObject->Credits=Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{

	if(SaveObject)
	{
		Credits= SaveObject->Credits;
		
	}
}
