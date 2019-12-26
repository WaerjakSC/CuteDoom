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
