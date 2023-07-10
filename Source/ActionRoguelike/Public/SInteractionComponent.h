// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWorldUserWidget.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	void PrimaryInteract();

	// Sets default values for this component's properties
	USInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestInteractable();

	//Reliable - Will always arrive, eventually.
	//Unreliable - not guaranteed, packet can get lost and wont retry.
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus); //the interactable being pointed to by the client. sent to server as a parameter.

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
