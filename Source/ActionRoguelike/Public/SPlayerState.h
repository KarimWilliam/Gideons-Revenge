// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASPlayerState();
	


	
protected:
	UPROPERTY(EditDefaultsOnly,Category="PlayerState",Replicated)
	int32 Credits;
public:
	UFUNCTION(BlueprintCallable)
	int GetCredits() const;
	void SetCredits(int creds);
	bool ApplyCreditChange(int creds);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);
	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);
	
};
