// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "MySAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMySAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackAnimDelay;
	
	UPROPERTY(EditAnywhere, Category="Effects")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorActor);

public:
	UMySAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
