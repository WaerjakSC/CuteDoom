// Fill out your copyright notice in the Description page of Project Settings.


#include "CuteGameplayHUD.h"

void UCuteGameplayHUD::NativeConstruct()
{
	Super::NativeConstruct();
	if (CrosshairImage)
	{
		CrosshairImage->SetBrushFromTexture(CrosshairTex);
	}
}

void UCuteGameplayHUD::SynchronizeProperties()
{
	if (CrosshairImage)
	{
		CrosshairImage->SetBrushFromTexture(CrosshairTex, true);
	}
}
