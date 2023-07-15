// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/OnlineEngineInterface.h"
#include "SAttributesComponent.generated.h"

class USAttributesComponent;

//Create a custom event that notifies subscribers when something happens (avoids checking with ticks) IMPORTANT
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributesComponent*,
                                              OwningComp, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	static USAttributesComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(AdvancedDisplay="IsAlive"))
	static bool IsActorAlive(AActor* Actor);


	// Sets default values for this component's properties
	USAttributesComponent();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes",Replicated)
	float Health;
	void SetMaxHealth(float MaxH);
	float GetMaxHealth();
	float GetHealth();
	float GetMaxRage();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes",Replicated)
	float Rage;


protected:

	UFUNCTION(NetMulticast,Reliable) //fire and forget across clients.
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes", Replicated)
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes", Replicated)
	float MaxRage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
};
