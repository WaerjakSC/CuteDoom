// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shotgun.generated.h"

/**
 *
 */
UCLASS()
class CUTEDOOM_API UShotgun final : public UWeapon
{
	GENERATED_BODY()
public:
	float GetSpread() const
	{
		return Spread;
	}

private:
	// Shotgun spread in degrees to each side of the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float Spread{15.f};
};
