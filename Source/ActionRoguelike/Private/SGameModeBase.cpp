// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributesComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

//create console command to stop the spawning of bots.
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer."),ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval=5.0f; // run a timer to try and spawn ots every 5 seconds if not enough are alive according to the difficulty curve.
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimeHande_SpawnBots,this,&ASGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);
}

void ASGameModeBase::KillAll()
{
		for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) //TActorIterator returns every actor of the given class in our current level.
			{
			ASAICharacter* Bot= *It;

			USAttributesComponent* AttributesComp=  USAttributesComponent::GetAttributes(Bot);

			if(AttributesComp && AttributesComp->IsAlive())
			{
				AttributesComp->Kill(this);
			}
		}
}




void ASGameModeBase::SpawnBotTimerElapsed()
{
	if(CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("SBot spawning disabbled via cvar 'CVSpawnots'."))
		return;
	}

	//Maximum of 5 bots allive at a time
	int32 NrOfAliveBots=0;
	for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) //TActorIterator returns every actor of the given class in our current level.
		{
		ASAICharacter* Bot= *It;

		USAttributesComponent* AttributesComp=  USAttributesComponent::GetAttributes(Bot);

		if(AttributesComp && AttributesComp->IsAlive())
		{
			NrOfAliveBots++;
		}
		}

	float MaxBotCount=5.0f;

	if(DifficultyCurve)
	{
		MaxBotCount=DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	
	if(NrOfAliveBots>=MaxBotCount)
	{
		return;
	}

	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance= UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
	}
	
}



void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus!=EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn bot EQS Query Failed"))
		return;
	}



	TArray<FVector> Locations= QueryInstance->GetResultsAsLocations();

	if(Locations.Num()>0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	if(ASCharacter* Player= Cast<ASCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_TRespawnDelay;
		//Delegate is needed when the function hat will fire after the timer elapses has input variables.
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());
		float RespawnDelay=2.0;
		GetWorldTimerManager().SetTimer(TimerHandle_TRespawnDelay,Delegate,RespawnDelay,false);
	}


if(ASAICharacter* Bot= Cast<ASAICharacter>(VictimActor))
	{
		if(ASCharacter* PC= Cast<ASCharacter>(Killer))
		{
			if(ASPlayerState* PlayerState=Cast<ASPlayerState> (PC->GetPlayerState()))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("we earned some mulla"));	
				PlayerState->ApplyCreditChange(100);
			}
		}
	}
	UE_LOG(LogTemp,Log,TEXT("OnActorKiller Victim: %s, Killer: %s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
	
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}