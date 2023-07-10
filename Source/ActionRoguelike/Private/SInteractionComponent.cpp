// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("su.InteractionDebugDraw"), true,TEXT("Enable Deug Lines for interact component"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams,
	                                                       Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		// if (bDebugDraw)
		// {
		// 	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		// }

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}


	// if (bDebugDraw)
	// {
	// 	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 0.0f, 0, 0.0f);
	// }
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (const APawn* MyPawn = Cast<APawn>(GetOwner()); MyPawn->IsLocallyControlled()) //code only runs for the locally controlled actor.
	{
		FindBestInteractable();
		
	}
}


void USInteractionComponent::PrimaryInteract() 
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No focused actor to interact with"));
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}
