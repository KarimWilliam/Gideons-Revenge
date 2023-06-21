// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

class UParticleSystemComponent;

ASTeleportProjectile::ASTeleportProjectile()
{
	DetEffectComp=CreateDefaultSubobject<UParticleSystemComponent>("DetEffectComp");
	DetEffectComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnHit);
	GetWorldTimerManager().SetTimer(TimerHandle_DashAttack,this,&ASTeleportProjectile::Detonate, 0.8f);
	
}

void ASTeleportProjectile::Detonate()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Art is an explosion"));
	MovementComp->Velocity=FVector(0,0,0);
	DetEffectComp->Activate();
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAttack,this,&ASTeleportProjectile::Teleport, 0.2f);
}

void ASTeleportProjectile::Teleport()
{
	Destroy();
	GetInstigator()->SetActorLocation(GetActorLocation(),false); //Better to use TeleportTo()
	const FRotator NewRot (GetInstigator()->GetController()->GetControlRotation().Pitch,GetActorRotation().Yaw,GetInstigator()->GetController()->GetControlRotation().Roll);
	GetInstigator()->GetController()->SetControlRotation(NewRot);
}

void ASTeleportProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DashAttack); // Clear the timer
	Detonate();
	
}