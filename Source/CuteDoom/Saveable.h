// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Saveable.generated.h"

/**
 *
 */
UINTERFACE(Blueprintable)
class CUTEDOOM_API USaveable : public UInterface
{
	GENERATED_UINTERFACE_BODY()


};

class ISaveable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
		void ActorSaveDataLoaded();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
		void ActorSaveDataSaved();
};
