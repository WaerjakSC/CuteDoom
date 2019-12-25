// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "EnemyBase.generated.h"

class UWeapon;
UCLASS()
class CUTEDOOM_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	float Health{100};

	TSubclassOf<AActor> Meat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyCharacter)
	bool bIsDead{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttacking{false};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentCooldown{.12f};
	UPROPERTY(BlueprintReadOnly)
	float AttackCooldown{.18f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* BloodGush;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* Gibs;
	/** Called by external actors when they want to apply damage and/or force. */
	void HitEvent(const FVector AttackPosition, const float Damage, const float ForceScaling);
	/** Alternate function taking a weapon type instead. */
	void HitEvent(const FVector AttackPosition, const UWeapon* Weapon);
	UFUNCTION(BlueprintCallable)
	void SpawnMeat();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* EnemyMesh;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Collider;
	UPROPERTY(VisibleAnywhere)
	class UCharacterMovementComponent* Movement;
public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	bool bIsRagdoll{false};
};
