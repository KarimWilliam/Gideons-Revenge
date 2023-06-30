// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	Credits=0;
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
