// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributesComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{

	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AttributeComp=CreateDefaultSubobject<USAttributesComponent>("AttributeComp");

	TimeToHit="TimeToHit";

	ActionComp =CreateDefaultSubobject<USActionComponent>("ActionComp");
	//have projectiles hit the mesh instead of the capsule.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this,& ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this,&ASAICharacter::OnHealthChanged); // binds Onhealthchanged that is defined in the attributes comp to this actor.
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	if(AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",NewTarget);
	}
}


void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);

		//DrawDebugString(GetWorld(),GetActorLocation(),"Payer Spotted",nullptr,FColor::White,4.f,true);
	}


void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0) 
	{
		if(InstigatorActor !=this) //if damage came from not self set our new target to the one that damaged us.
		{
			SetTargetActor(InstigatorActor);
		}

		if(ActiveHealthBar==nullptr)
		{
				ActiveHealthBar= CreateWidget<USWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
            		if(ActiveHealthBar)
            		{
            			ActiveHealthBar->AttachedActor=this;
            			ActiveHealthBar->AddToViewport();
            		}
		}
	
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHit,GetWorld()->TimeSeconds);
		
		if(NewHealth <= 0.0f)//he died
		{
			//Stop BT
			if(AAIController* AIC = Cast<AAIController>(GetController()))
			{
				AIC->GetBrainComponent()->StopLogic("Killed"); //brain component holds the behavior tree. parent class.
			}
			//Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			//set lifespan
			SetLifeSpan((10.0));
		}
	}
}


