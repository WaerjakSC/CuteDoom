// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class CUTEDOOM_API UInteractable : public UInterface
{
	GENERATED_BODY()
	
};

class IInteractable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Gameplay)
		void Activate();
};
