// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponStats.generated.h"

class USoundBase;
class UAnimMontage;
/**
 *
 */
UCLASS(Blueprintable)
class CUTEDOOM_API UWeaponStats : public UObject
{
	GENERATED_BODY()
public:

	float GetRange() const { return Range; }
	float GetDamage() const { return Damage; }
	USoundBase* GetSound() const { return FireSound; }
	UAnimMontage* GetAnimation() const { return FireAnimation; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float Range{ 2500.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float Damage{ 50.f };
	// Total amount of ammo.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int Ammo{ 100 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int AmmoPerClip{ 10 };
	// Time between each shot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float Cooldown{ 0.2f };
	// How much force each bullet will impart on hitting a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float ShotForce{ 4000.f };
	// How many shots a gun should fire per attack - A shotgun might have 5-10 pellets in one shot for instance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int ShotsPerAttack{ 1 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UAnimMontage* FireAnimation;
};
