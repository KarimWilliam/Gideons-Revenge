// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "SAttributesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MaxBulletSpread=2.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController =OwnerComp.GetAIOwner();
	if(ensure(AIController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(AIController->GetPawn());
		if(MyPawn==nullptr)
		{
			return EBTNodeResult::Failed;
		}
		FVector MuzzleLocation= MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor= Cast<AActor>( OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if(TargetActor==nullptr)
		{
			return EBTNodeResult::Failed; 
		}
		
		if(!USAttributesComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		
		//Target - origin vectors apparenty gives a direction vector? 
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation= Direction.Rotation();

		MuzzleRotation.Pitch+=FMath::RandRange(0.f,MaxBulletSpread);
		MuzzleRotation.Yaw+=FMath::RandRange(-MaxBulletSpread,MaxBulletSpread);

		
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator=MyPawn;
 
		const AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,Params);
		return NewProj? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

