// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponStats.h"
#include "CuteCharacter.generated.h"

/*
Contains the various weapon types for use as a faux-FiniteStateMachine
*/
UENUM(BlueprintType)
enum class ESelectedWeapon : uint8
{
	Pistol,
	Shotgun,
	Unknown
};

UCLASS()
class CUTEDOOM_API ACuteCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACuteCharacter();

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetArmsMesh() const { return ArmsMesh; }

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Fires a projectile. */
	void Shoot();

	/** Kick event */
	void Kick();

	/** Interaction event */
	void Interact();

	UFUNCTION(BlueprintCallable)

	void SetWeapon(ESelectedWeapon NewWeapon);
	// Character health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCharacter)
	float MaxHealth{100};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCharacter)
	float Health{100};
	// Total boost from eating meat.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Meat)
	float MeatBoost{0.f}; // Number of meat pieces to next boost.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Meat)
	int ToNextBoost{10};
	// Total amount of meat pieces eaten.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Meat)
	float MeatsEaten{100};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Debug)
	bool bDebugMode{false};
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate.   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	ESelectedWeapon CurrentWeapon{ESelectedWeapon::Pistol};
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FVector GunOffset;
	UPROPERTY(BlueprintReadOnly)
	UWeaponStats* CurWeapon{nullptr};
	UPROPERTY(BlueprintReadOnly)
	UWeaponStats* KickWeapon{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<UWeaponStats> PistolBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<UWeaponStats> ShotgunBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<UWeaponStats> KickBP;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	bool DoTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams, float TraceRange);

	/** Handles moving forward/backward */
	void MoveForward(float Value);

	/** Handles strafing movement, left and right */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired
	 * turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired
	 * turn rate
	 */
	void LookUpAtRate(float Rate);

private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* ArmsMesh;
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FPGun;

	/** First person camera */
	UPROPERTY(VisibleAnywhere,
		BlueprintReadOnly,
		Category = Camera,
		meta = (AllowPrivateAccess = "true"))

	class UCameraComponent* FirstPersonCameraComponent;


public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;
};
