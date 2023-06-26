// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "SAttributesComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BlackboardComponent= OwnerComp.GetBlackboardComponent();

	if(ensure( BlackboardComponent))
	{
			AAIController* AIController= OwnerComp.GetAIOwner();
			if(ensure(AIController))
			{
			
				ASAICharacter* Self= Cast<ASAICharacter>(AIController->GetPawn());
				if(ensure(Self))
				{
					USAttributesComponent* AttributeComp= Cast<USAttributesComponent>(Self->GetComponentByClass(USAttributesComponent::StaticClass()));
		
					float Health= AttributeComp->GetHealth();
					BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName,(Health<=50));
				}

			}
			}
		
	
	}
	
	

