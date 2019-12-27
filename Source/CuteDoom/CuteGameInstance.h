// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CuteGameInstance.generated.h"

class UCuteSaveGame;
class USaveSlotSettings;
/**
 * 
 */
UCLASS()
class CUTEDOOM_API UCuteGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	explicit UCuteGameInstance(const FObjectInitializer& ObjectInitializer);
	void CreateNewSave(const FString& SaveName);
	void LoadSaveGame(const FString& SaveName);
	UCuteSaveGame* LoadCurrentSave();
	void SaveCurrentToSlot();
	/**
	 * @brief Get all the save slot names in existence.
	 * We assume UserIndex will always be 0, so we can simply get a list of all the save slot names to display and load in a UMG Widget.
	 */
	TSet<const FString> GetAllSaveNames() const;

	FString GetCurrentSaveName() const
	{
		return CurrentSaveName;
	}

	void SetCurrentSaveName(const FString& NewName) { CurrentSaveName = NewName; }
	
	void Shutdown() override;
	void SaveActors(const FString& SaveSlotName) const;
	void LoadActors() const;
private:
	UPROPERTY()
	FString CurrentSaveName;
	UPROPERTY()
	UCuteSaveGame* CurrentSave{nullptr};
	UPROPERTY()
	USaveSlotSettings* SaveInfo{nullptr};
};
