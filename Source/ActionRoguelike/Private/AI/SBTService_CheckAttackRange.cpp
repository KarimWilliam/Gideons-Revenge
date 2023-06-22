// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//check distance between AI pawn and target actor.

	UBlackboardComponent* BlackboardComponent=	OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComponent))
	{
		AActor* TargetActor= Cast<AActor>( BlackboardComponent->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* AIController= OwnerComp.GetAIOwner();
			if(ensure (AIController))
			{
				APawn* AIPawn= AIController->GetPawn();
				if(ensure(AIPawn))
				{
					float DistanceTo=FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());
					bool bHasLOS=false;
					bool bWithinRange=DistanceTo<2000.f;

					if(bWithinRange)
					{
						bHasLOS= AIController->LineOfSightTo(TargetActor);
					}

					//BlackboardComponent->SetValueAsBool("WithinAttackRange",bWithinRange); Lets us selelct the variable on the fly from the tree.
					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName,(bWithinRange && bHasLOS));
				}
			}
		}
	}
	
}
