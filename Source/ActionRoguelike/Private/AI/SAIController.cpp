// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree,TEXT("Behacior Reww is nullptr. Please assign BehaviorTree in your AI Controller dummy")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	


	// APawn* MyPawn=	UGameplayStatics::GetPlayerPawn(this,0);
	// if(MyPawn)
	// {
	//
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation",MyPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	// }


}
