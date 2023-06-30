// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASPlayerState();
	


	
protected:
	UPROPERTY(EditDefaultsOnly,Category="PlayerState")
	int Credits;
public:
	UFUNCTION(BlueprintCallable)
	int GetCredits() const;
	void SetCredits(int creds);
	bool ApplyCreditChange(int creds);
	
};
