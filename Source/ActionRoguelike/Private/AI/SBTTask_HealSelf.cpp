// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"
#include "AIController.h"
#include "SAttributesComponent.h"
#include "AI/SAICharacter.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController =OwnerComp.GetAIOwner();
	if(ensure(AIController))
	{
		ASAICharacter* Self= Cast<ASAICharacter>(AIController->GetPawn());
		USAttributesComponent* AttributeComp= Cast<USAttributesComponent>(Self->GetComponentByClass(USAttributesComponent::StaticClass()) );
		AttributeComp->ApplyHealthChange(Self,50.f);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

