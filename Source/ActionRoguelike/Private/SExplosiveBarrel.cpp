// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	CapsuleComp= CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	RootComponent=CapsuleComp;
	
	MeshComp-> SetupAttachment(CapsuleComp);
	MeshComp-> SetCollisionProfileName("PhysicsActor");
	MeshComp -> SetSimulatePhysics(true);
	
	RadialForceComp= CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp -> Radius = 500.0f;
	RadialForceComp -> ImpulseStrength = 2000;
	RadialForceComp -> bImpulseVelChange = true;

	//Radial Explosion
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHit);


}

void ASExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

