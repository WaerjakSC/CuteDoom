// Fill out your copyright notice in the Description page of Project Settings.


#include "CuteSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Saveable.h"
#include "Engine/World.h"


UCuteSaveGame::UCuteSaveGame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCuteSaveGame::SaveActors()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveable::StaticClass(), Actors);

	for (auto Actor : Actors)
	{
		FActorSaveData ActorRecord;
		ActorRecord.ActorName = FName(*Actor->GetName());
		ActorRecord.ActorClass = Actor->GetClass()->GetPathName();
		ActorRecord.ActorTransform = Actor->GetTransform();

		FMemoryWriter MemoryWriter(ActorRecord.ActorData, true);
		FSaveGameArchive Ar(MemoryWriter);
		Actor->Serialize(Ar);

		SavedActors.Add(ActorRecord);
		ISaveable::Execute_ActorSaveDataSaved(Actor);
	}
}

void UCuteSaveGame::LoadActors()
{
	for (FActorSaveData ActorRecord : SavedActors)
	{
		// Should do some stuff here to determine if object was destroyed (or changed) since last save, and only in that case should we respawn/refill the actor with the given variables.
		// A checkpoint system might be nice, with tags on objects to denote which checkpoint it's part of.
		// So that if the player dies in one checkpoint we only need to check and reset the actors between that checkpoint and the next, instead of the whole level. 
		FVector SpawnPos = ActorRecord.ActorTransform.GetLocation();
		FRotator SpawnRot = ActorRecord.ActorTransform.Rotator();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Name = ActorRecord.ActorName;
		UClass* SpawnClass = FindObject<UClass>(ANY_PACKAGE, *ActorRecord.ActorClass);
		if (SpawnClass)
		{
			AActor* NewActor = GetWorld()->SpawnActor(SpawnClass, &SpawnPos, &SpawnRot, SpawnParams);
			FMemoryReader MemoryReader(ActorRecord.ActorData, true);
			FSaveGameArchive Ar(MemoryReader);
			NewActor->Serialize(Ar);
			NewActor->SetActorTransform(ActorRecord.ActorTransform);
			ISaveable::Execute_ActorSaveDataLoaded(NewActor);
		}
	}
}
