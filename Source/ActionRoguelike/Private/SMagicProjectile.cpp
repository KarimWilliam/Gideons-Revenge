// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "GamePlayFunctionLibrary.h"
#include "SAttributesComponent.h"
#include "../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/spdlog/include/spdlog/fmt/bundled/color.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
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
		// USAttributesComponent* AttributeComp = Cast<USAttributesComponent>(
		// 	OtherActor->GetComponentByClass(USAttributesComponent::StaticClass()));

		if(UGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,Damage,HitResult))
		{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("we hit something that has attributes"));
			 	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound->GetSound(), GetActorLocation(), 0.2f);
			 	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 100, 500);
			 	Destroy();
		}
	}
}

//NOT IN EFFECT NO BINDING. LOGIC IN BLUEPRINTS
void ASMagicProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("we hit something that has no attributes"));
	Destroy();
}
