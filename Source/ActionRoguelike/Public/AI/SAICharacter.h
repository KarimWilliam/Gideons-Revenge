// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

class USAttributesComponent;
UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();


protected:
	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHit;
	
	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributesComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;
	
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION() 
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget>HealthBarWidgetClass;

	USWorldUserWidget* ActiveHealthBar;
};
