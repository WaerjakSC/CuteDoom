// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.generated.h"

class USoundBase;
class UAnimMontage;
/**
 *
 */
UCLASS(Blueprintable)
class CUTEDOOM_API UWeapon : public UObject
{
	GENERATED_BODY()
public:

	float GetRange() const { return Range; }

	float GetDamage() const { return Damage; }

	float GetForce() const { return ShotForce; }

	int GetAmmo() const { return Ammo; }

	int GetSpareAmmo() const { return SpareAmmo; }

	int GetAmmoPerClip() const { return AmmoPerClip; }

	float GetCooldown() const { return Cooldown; }

	int GetHitsPerAttack() const { return HitsPerAttack; }

	USoundBase* GetSound() const { return FireSound; }

	UAnimMontage* GetAnimation() const { return FireAnimation; }
	/**
	 * @brief Depletes ammo if there's any left in the clip, otherwise does nothing.
	 */
	void UseAmmo();
	/**
	 * @brief Reloads the clip with as much as is needed to fill it to capacity. Assumes no loss of ammo from reloading a non-empty clip.
	 */
	void Reload();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float Range{2500.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float Damage{50.f};
	// Total amount of ammo not in clip.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int SpareAmmo{50};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int Ammo{10};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int AmmoPerClip{10};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int MaxAmmo{100};
	// Time between each shot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float Cooldown{0.2f};
	// How much force each bullet will impart on hitting a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float ShotForce{4000.f};
	// How many shots a gun should fire per attack - A shotgun might have 5-10 pellets in one shot for instance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int HitsPerAttack{1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage* FireAnimation;
};
