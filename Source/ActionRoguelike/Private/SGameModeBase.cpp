// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributesComponent.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

//create console command to stop the spawning of bots.
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true,TEXT("Enable spawning of bots via timer."),
                                                ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 5.0f;

	SlotName = "SaveGame01";
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimeHande_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	//TActorIterator returns every actor of the given class in our current level.
	{
		ASAICharacter* Bot = *It;

		USAttributesComponent* AttributesComp = USAttributesComponent::GetAttributes(Bot);

		if (AttributesComp && AttributesComp->IsAlive())
		{
			AttributesComp->Kill(this);
		}
	}
}


void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("SBot spawning disabbled via cvar 'CVSpawnots'."))
		return;
	}

	//Maximum of 5 bots allive at a time
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	//TActorIterator returns every actor of the given class in our current level.
	{
		ASAICharacter* Bot = *It;

		USAttributesComponent* AttributesComp = USAttributesComponent::GetAttributes(Bot);

		if (AttributesComp && AttributesComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 5.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}


	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}


	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}


void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"))
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	if (ASCharacter* Player = Cast<ASCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_TRespawnDelay;
		//Delegate is needed when the function hat will fire after the timer elapses has input variables.
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		float RespawnDelay = 2.0;
		GetWorldTimerManager().SetTimer(TimerHandle_TRespawnDelay, Delegate, RespawnDelay, false);
	}


	if (ASAICharacter* Bot = Cast<ASAICharacter>(VictimActor))
	{
		if (ASCharacter* PC = Cast<ASCharacter>(Killer))
		{
			if (ASPlayerState* PlayerState = Cast<ASPlayerState>(PC->GetPlayerState()))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("we earned some mulla"));	
				PlayerState->ApplyCreditChange(100);
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKiller Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);`
	}
}

//GAME SAVE/LOAD

void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]); //finds the player state to write.
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame); //Writes our current player state in the save object.
			break;
		}
	}

	CurrentSaveGame->SavedActors.Empty(); //empty the list of actor locations before we save them again.

	//iterate through all actors in the world to save them
	for(FActorIterator It(GetWorld());It;++It) 
	{
		AActor* Actor= *It;
		// Only interested in gameplay actors
		if(!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();


		//save game tag resolution
		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame=true; //this marks it so it only looks for the UPROPERTY(SaveGame) property.
		//Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);


		
		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
	//writes the  savegame object to disk under the name SlotName.
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		//get the savegame object from disk to memory. 
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Load SaveGame Data."));
			return;
		}
		//  The next step would be to place it into my player-state object BUT i cant do it here because this function is called super early before player state is initlized. so i need to delay giving the info to the player-state right away.
		//instead we do that in  HandleStartingNewPlayer_Implementation below.
		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data Successfully."));

		//load world objects in proper places from my saved FActorSaveData array to each actor in the world.
		for(FActorIterator It(GetWorld());It;++It) //iterate through entire world of actors
		{
			AActor* Actor= *It;
			// Only interested in gameplay actors
			if(!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for(FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
			 if(ActorData.ActorName == Actor->GetName())
			 {
				 Actor->SetActorTransform(ActorData.Transform);


			 	//opposite of serilization saving
			 	FMemoryReader MemReader(ActorData.ByteData);
			 	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
			 	Ar.ArIsSaveGame=true; //this marks it so it only looks for the UPROPERTY(SaveGame) property.
			 	Actor->Serialize(Ar); //Converts Binary array back into actor's variables

			 	ISGameplayInterface::Execute_OnActorLoaded(Actor);
			 	break;
			 }
			}
		}
	}
	else

	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data"));
	}
	
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
//Call LoadSaveGame ASAP. InitGame happens very early in the chain.
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) //Signals a player is ready to enter the game
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}
