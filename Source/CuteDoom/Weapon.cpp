// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

void UWeapon::UseAmmo()
{
	if (Ammo == 0)
	{
		return;
	}
	// We only ever use one ammo per attack. Weapons that shoot more than one projectile at a time can just spoof it by having one ammo be equal to however many shots they fire.
	Ammo -= 1;
}

void UWeapon::Reload()
{
	if (SpareAmmo == 0) // No ammo left, don't do anything.
	{
		return;
	}
	const int NeededAmmo{GetAmmoPerClip() - Ammo};
	const int ToFillClip{NeededAmmo <= SpareAmmo ? NeededAmmo : SpareAmmo};
	Ammo += ToFillClip;
	SpareAmmo -= ToFillClip;
}
