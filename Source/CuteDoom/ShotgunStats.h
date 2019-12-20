// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponStats.h"
#include "ShotgunStats.generated.h"

/**
 *
 */
UCLASS()
class CUTEDOOM_API UShotgunStats : public UWeaponStats
{
	GENERATED_BODY()
private:
	// Shotgun spread in degrees to each side of the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float Spread{ 15.f };
};
