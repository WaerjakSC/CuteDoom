// Fill out your copyright notice in the Description page of Project Settings.


#include "CuteGameInstance.h"
#include "SaveSlotSettings.h"
#include "Kismet/GameplayStatics.h"
#include "CuteSaveGame.h"
#include "Saveable.h"
#include "Engine/World.h"


UCuteGameInstance::UCuteGameInstance(const FObjectInitializer& ObjectInitializer): UGameInstance(ObjectInitializer)
{
	SaveInfo = Cast<USaveSlotSettings>(UGameplayStatics::LoadGameFromSlot("SaveSlotSettings", 0));
	// Load the save slot info.
	if (SaveInfo)
	{
		CurrentSaveName = SaveInfo->LastSave;
		CurrentSave = Cast<UCuteSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveName, 0));
		if (!CurrentSave)
		{
			CreateNewSave("New Save");
		}
	}
}

void UCuteGameInstance::CreateNewSave(const FString& SaveName)
{
	CurrentSave = Cast<UCuteSaveGame>(UGameplayStatics::CreateSaveGameObject(UCuteSaveGame::StaticClass()));
	CurrentSaveName = SaveName;
	CurrentSave->SaveSlotName = CurrentSaveName;
}

void UCuteGameInstance::LoadSaveGame(const FString& SaveName)
{
	if (UCuteSaveGame* LoadedGame{Cast<UCuteSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0))})
	{
		CurrentSave = LoadedGame;
		CurrentSaveName = SaveName;
		LoadCurrentSave(); // Actually load the info from the SaveGame file into the world. 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load save file: %s"), *SaveName);
	}
}

UCuteSaveGame* UCuteGameInstance::LoadCurrentSave()
{
	LoadActors();
	return Cast<UCuteSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveName, 0));
}

void UCuteGameInstance::SaveCurrentToSlot()
{
	if (CurrentSave)
	{
		SaveActors(CurrentSaveName);
		// Maybe save some other info as well.
	}
}

TSet<const FString> UCuteGameInstance::GetAllSaveNames() const
{
	return SaveInfo->SaveSlotNames;
}

void UCuteGameInstance::Shutdown()
{
	// I have a feeling actors may be destroyed before this is called, but putting this here for now.
	SaveCurrentToSlot(); // Save the game before quitting. We always set the current save to be the one last saved to, so this should just update that file.
}

void UCuteGameInstance::SaveActors(const FString& SaveSlotName) const
{
	UCuteSaveGame* SaveSlot{Cast<UCuteSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0))};
	if (!SaveSlot)
	{
		return;
	}
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USaveable::StaticClass(), Actors);
	SaveSlot->SavedActors.Empty(); // Clear the array so we don't get duplicates.
	for (auto Actor : Actors)
	{
		FActorSaveData ActorRecord;
		ActorRecord.ActorName = FName(*Actor->GetName());
		ActorRecord.ActorClass = Actor->GetClass()->GetPathName();
		ActorRecord.ActorTransform = Actor->GetTransform();

		FMemoryWriter MemoryWriter(ActorRecord.ActorData, true);
		FSaveGameArchive Ar(MemoryWriter);
		Actor->Serialize(Ar);

		SaveSlot->SavedActors.Add(ActorRecord);
		ISaveable::Execute_ActorSaveDataSaved(Actor);
	}
	if(!UGameplayStatics::SaveGameToSlot(SaveSlot, SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save file to slot!"));
	}
}

void UCuteGameInstance::LoadActors() const
{
	for (FActorSaveData ActorRecord : CurrentSave->SavedActors)
	{
		// TODO: Should do some stuff here to determine if object was destroyed (or changed) since last save, and only in that case should we respawn/refill the actor with the given variables.
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
