// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation=true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	
	GetCharacterMovement()->bOrientRotationToMovement=true;
	
	bUseControllerRotationYaw=false;

	InteractionComp =CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent-> BindAxis("MoveForward",this, &ASCharacter::MoveForward);
	PlayerInputComponent-> BindAxis("MoveRight",this, &ASCharacter::MoveRight);
	
	PlayerInputComponent-> BindAxis("Turn",this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent-> BindAxis("Lookup",this, &ASCharacter::AddControllerPitchInput);

	PlayerInputComponent-> BindAction("Jump",IE_Pressed,this,&ASCharacter::Jump);
	PlayerInputComponent-> BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
	PlayerInputComponent-> BindAction("PrimaryInteract",IE_Pressed,this,&ASCharacter::PrimaryInteract);
	PlayerInputComponent-> BindAction("SpecialAttack",IE_Pressed,this,&ASCharacter::SpecialAttack);
	
}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot= GetControlRotation();
	ControlRot.Pitch=0.0f;
	ControlRot.Roll=0.0f;
	
	AddMovementInput(ControlRot.Vector(),value);
}

void ASCharacter::MoveRight(float value)
{
	/*
	 X= Forward ROLL
	 Y= Right PITCH
	 Z= up YAW
	*/
	
	FRotator ControlRot= GetControlRotation();
	ControlRot.Pitch=0.0f;
	ControlRot.Roll=0.0f;
	//ControlRot.Yaw+=90.0f;
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector,value);
}

void ASCharacter::PrimaryAttack()
{
	
	PlayAnimMontage(AttackAnim);

	//Creates a timer
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack); // Clear the timer


}

void ASCharacter::PrimaryAttack_TimeElapsed()
{



		if(ensureAlways(ProjectileClass))
		{
			FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Instigator=this; //sets the Instigator of the projectile in the spawn params (for kill credit as an example)
	
			

			FHitResult Hit;
			FCollisionShape Shape;
			Shape.SetSphere(20.0f);

			//ignore player
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			
			FCollisionObjectQueryParams ObjParams;
			ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			ObjParams.AddObjectTypesToQuery(ECC_Pawn);
			ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

			FVector TraceStart=CameraComp->GetComponentLocation();
			FVector TraceEnd= TraceStart+(CameraComp->GetComponentRotation().Vector()*5000);
			
			if(GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
			{
				//set the end to the hit point
				TraceEnd=Hit.ImpactPoint;
			}
			//Find new direction/rotation from Hand pointing to impact in world;
			FRotator ProjRotation= FRotationMatrix::MakeFromX(TraceEnd-HandLocation).Rotator();
			DrawDebugLine(GetWorld(),HandLocation,TraceEnd, FColor::Green ,false,2.0f,0,2.0f);

			FTransform SpawnTM=FTransform(ProjRotation,HandLocation);
			GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM,SpawnParams);
		}

}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::SpecialAttack()
{

	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&ASCharacter::SpecialAttack_TimeElapsed, 0.2f);
	
}

void ASCharacter::SpecialAttack_TimeElapsed()
{



	if(ensureAlways(BlackholeClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator=this; //sets the Instigator of the projectile in the spawn params (for kill credit as an example)
	
			

		FHitResult Hit;
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//ignore player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
			
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FVector TraceStart=CameraComp->GetComponentLocation();
		FVector TraceEnd= TraceStart+(CameraComp->GetComponentRotation().Vector()*5000);
			
		if(GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
		{
			//set the end to the hit point
			TraceEnd=Hit.ImpactPoint;
		}
		//Find new direction/rotation from Hand pointing to impact in world;
		FRotator ProjRotation= FRotationMatrix::MakeFromX(TraceEnd-HandLocation).Rotator();
		DrawDebugLine(GetWorld(),HandLocation,TraceEnd, FColor::Green ,false,2.0f,0,2.0f);

		FTransform SpawnTM=FTransform(ProjRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(BlackholeClass,SpawnTM,SpawnParams);
	}

}
