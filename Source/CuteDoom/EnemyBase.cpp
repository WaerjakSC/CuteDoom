// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = GetCapsuleComponent();
	Collider->InitCapsuleSize(36.f, 130.f);

	EnemyMesh = GetMesh();
	BloodGush = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Blood Hit-PFX"));
	BloodGush->SetupAttachment(RootComponent);
	BloodGush->SetAutoActivate(false);

	Gibs = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GIBS"));
	Gibs->SetupAttachment(RootComponent);
	Gibs->SetAutoActivate(false);

	EnemyMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	EnemyMesh->SetCollisionObjectType(ECC_PhysicsBody);
	//EnemyMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	EnemyMesh->SetEnableGravity(true);
	EnemyMesh->SetSimulatePhysics(false);

	Movement = GetCharacterMovement();
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::HitEvent(APawn* Actor, FPointDamageEvent& DamageEvent, const UWeapon* Weapon)
{
	if (bIsDead) // shoot as many times as you like to spawn as many gibs as you like I guess, should be funny
	{
		Gibs->ActivateSystem();
		EnemyMesh->DestroyComponent();
		return;
	}
	if (Health > 0)
	{
		Health -= TakeDamage(Weapon->GetDamage(), DamageEvent, Actor->GetController(), Actor);
		// From the ShotDirection, determine which rotation the blood spurt should have.
		const FRotator RotateToAttacker{DamageEvent.ShotDirection.ToOrientationRotator()};
		BloodGush->SetRelativeRotation(RotateToAttacker);
		BloodGush->ActivateSystem();
	}
	if (Health <= 0.f)
	{
		bIsDead = true;
		/* Disable all collision on capsule */
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
		if (!bIsRagdoll)
		{
			if(EnemyMesh)
			{
				EnemyMesh->SetCollisionProfileName(TEXT("Ragdoll"));
			}
			SetActorEnableCollision(true); // Collider is disabled, so we enable it for the whole actor instead.
			SetRagdollPhysics();
		}


		// Apply force from the attack.
		const FVector HitDirection{DamageEvent.ShotDirection * Weapon->GetForce()};
		// HitDirection.Z *= 1.4f;
		// Add some extra force in the Z direction to simulate the "flying backwards and up" trope in movies when people get shot
		EnemyMesh->AddImpulse(HitDirection);


		SpawnMeat();
	}
}

void AEnemyBase::SetRagdollPhysics()
{
	if (IsPendingKill())
	{
		bIsRagdoll = false;
	}
	else if (!EnemyMesh || !EnemyMesh->GetPhysicsAsset())
	{
		bIsRagdoll = false;
	}
	else
	{
		EnemyMesh->SetAllBodiesSimulatePhysics(true);
		EnemyMesh->SetSimulatePhysics(true);
		EnemyMesh->WakeAllRigidBodies();
		EnemyMesh->bBlendPhysics = true;

		bIsRagdoll = true;
	}

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}
	if (!bIsRagdoll)
	{
		// Immediately hide the pawn
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}

void AEnemyBase::SpawnMeat()
{
	if (Meat)
	{
		if (GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			const FRotator Rotation;

			FVector SpawnLocation = this->EnemyMesh->GetComponentLocation();
			const FVector IncreaseZ(0, 0, 2);
			SpawnLocation = SpawnLocation + IncreaseZ;
			GetWorld()->SpawnActor<AActor>(Meat, SpawnLocation, Rotation, SpawnParams);
		}
	}
}
