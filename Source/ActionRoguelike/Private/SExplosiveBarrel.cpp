// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	RootComponent = CapsuleComp;

	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetCollisionProfileName("PhysicsActor");
	MeshComp->SetSimulatePhysics(true);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 500.0f;
	RadialForceComp->ImpulseStrength = 2000;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

	//Radial Explosion
}

void ASExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	//Logging
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Art is an explosion"));
	UE_LOG(LogTemp, Log, TEXT("Art is a BOOM"))
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time :%f"), *GetNameSafe(OtherActor),
	       GetWorld()->TimeSeconds);
	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	//DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	RadialForceComp->FireImpulse();
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHit);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
