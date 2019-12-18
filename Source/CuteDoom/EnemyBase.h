// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "EnemyBase.generated.h"

UCLASS()
class CUTEDOOM_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float health { 100 };

	TSubclassOf<AActor> Meat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyCharacter)
		bool isDead{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAttacking{ false };
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float currentCooldown{ .12f };
	UPROPERTY(BlueprintReadOnly)
		float attackCooldown{ .18f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* bloodGush;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* gibs;
	void hitEvent(float damage, float forceScaling);
	UFUNCTION(BlueprintCallable)
		void spawnMeat();
	UFUNCTION(BlueprintImplementableEvent)
		void destroyFace();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* enemyMesh;
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* collider;
	UPROPERTY(VisibleAnywhere)
		class UCharacterMovementComponent* movement;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	bool bIsRagdoll{ false };
};
