// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CuteGameplayHUD.generated.h"

/**
 *
 */
UCLASS(Abstract)
class CUTEDOOM_API UCuteGameplayHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	/*
	This function is basically the c++ version of PreConstruct in blueprint. We can update stuff properly every time we compile.
	*/
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CrosshairImage;

	UPROPERTY(EditAnywhere)
		UTexture2D* CrosshairTex;
};
