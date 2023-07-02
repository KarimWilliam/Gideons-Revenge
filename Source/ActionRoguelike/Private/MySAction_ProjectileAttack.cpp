// Fill out your copyright notice in the Description page of Project Settings.


#include "MySAction_ProjectileAttack.h"

#include "SCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UMySAction_ProjectileAttack::UMySAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void UMySAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character=Cast<ASCharacter>(Instigator);
	if(Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector,
											   FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"AttackDelay_Elapsed",Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate,AttackAnimDelay,false);
	}

}

void UMySAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorActor)
{

	if (ensureAlways(ProjectileClass)) //WHAT IS PROJECTILE CLASS 
	{
		FVector HandLocation = InstigatorActor->GetMesh()->GetSocketLocation(HandSocketName);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//sets the Instigator of the projectile in the spawn params (for kill credit as an example)
		SpawnParams.Instigator = InstigatorActor;

		FHitResult Hit;
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//ignore player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorActor);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FVector TraceStart = InstigatorActor->GetPawnViewLocation();
		//FVector TraceEnd= TraceStart+(CameraComp->GetComponentRotation().Vector()*5000);
		FVector TraceEnd =TraceStart + (InstigatorActor->GetControlRotation().Vector() * 5000);

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			//set the end to the hit point
			TraceEnd = Hit.ImpactPoint;
		}
		//Find new direction/rotation from Hand pointing to impact in world;
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		DrawDebugLine(GetWorld(), HandLocation, TraceEnd, FColor::Green, false, 2.0f, 0, 2.0f);

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorActor);
}
