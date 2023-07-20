// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent=MeshComp;

	AttributeComp=CreateDefaultSubobject<USAttributesComponent>("AttributeComp");

	//Trigger when health is changed
	//Need to bind this event for it to activate for this particular actor. including an attribute comp doesnt activate it automatically.
	AttributeComp->OnHealthChanged.AddDynamic(this,&ASTargetDummy::OnHealthChanged); 

}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
	}
	 
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

