// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "GamePlayFunctionLibrary.h"
#include "SActionComponent.h"
#include "SAttributesComponent.h"
#include "../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/spdlog/include/spdlog/fmt/bundled/color.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	RootComponent = SphereComp;
	//Binds existing functions to my own function.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	//SphereComp->OnComponentHit.AddDynamic(this,&ASMagicProjectile::OnCompHit);
	AudioComp->SetupAttachment(SphereComp);
	ImpactSound->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	AudioComp->VolumeMultiplier = 0.2f;
	AudioComp->Play();
	//Ignores instigator collision
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
                                       UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg,
                                       const FHitResult& HitResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		 USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGamePlayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if(UGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,Damage,HitResult))
		{
			 	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound->GetSound(), GetActorLocation(), 0.2f);
			 	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 100, 500);

			//Burn Effect
			if(ActionComp)
			{
				ActionComp->AddAction(GetInstigator(),BurningActionClass);
			}
			 	Destroy();
		}
	}
}

//NOT IN EFFECT NO BINDING. LOGIC IN BLUEPRINTS
void ASMagicProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}
