// Fill out your copyright notice in the Description page of Project Settings.


#include "SGenericProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
ASGenericProjectile::ASGenericProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	//CameraShake=CreateDefaultSubobject<UCameraShakeBase>("CameraShake");

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationRemainsVertical = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");

	ImpactSound = CreateDefaultSubobject<UAudioComponent>("ImpactSound");


	//destroy actor after 10 seconds at the most.
	SetLifeSpan(10.0f);
}

// Called when the game starts or when spawned
void ASGenericProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASGenericProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
