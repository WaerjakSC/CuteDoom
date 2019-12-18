// Fill out your copyright notice in the Description page of Project Settings.


#include "CuteGameplayHUD.h"

void UCuteGameplayHUD::NativeConstruct() {
	Super::NativeConstruct();
	if (crosshairImage) {
		crosshairImage->SetBrushFromTexture(crosshairTex);
	}
}

void UCuteGameplayHUD::SynchronizeProperties()
{
	if (crosshairImage) {
		crosshairImage->SetBrushFromTexture(crosshairTex, true);
	}
}
