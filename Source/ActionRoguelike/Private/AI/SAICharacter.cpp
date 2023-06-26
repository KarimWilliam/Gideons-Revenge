// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "SAttributesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{

	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComp=CreateDefaultSubobject<USAttributesComponent>("AttributeComp");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this,& ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC= Cast<AAIController>(GetController());
	if(AIC)
	{
	    UBlackboardComponent* BBComp= AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor",Pawn);

		DrawDebugString(GetWorld(),GetActorLocation(),"Payer Spotted",nullptr,FColor::White,4.f,true);
	}
}


