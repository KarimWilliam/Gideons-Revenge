// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGenericProjectile.h"
#include "STeleportProjectile.generated.h"


class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASGenericProjectile
{
	GENERATED_BODY()
public:
	ASTeleportProjectile();
	
protected:
	virtual void BeginPlay() override;
	FTimerHandle TimerHandle_DashAttack;
	FTimerHandle TimerHandle_TeleportAttack;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UParticleSystemComponent* DetEffectComp;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void Detonate();
	void Teleport();
};
