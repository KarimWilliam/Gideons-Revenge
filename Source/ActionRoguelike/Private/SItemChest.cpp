// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASItemChest::ASItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
	TargetRoll = 0;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened(); //must manually call this for the server. it is only automatically called for clients.
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const //whenever bLidOpened changes on the server, send it to all clients.
{
	Super:: GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest,bLidOpened);
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetRoll : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(0, 0, CurrPitch));

}
