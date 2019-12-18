// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collider = GetCapsuleComponent();
	collider->InitCapsuleSize(36.f, 130.f);

	enemyMesh = GetMesh();
	bloodGush = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Blood Hit-PFX"));
	bloodGush->SetupAttachment(RootComponent);
	bloodGush->SetAutoActivate(false);

	gibs = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GIBS"));
	gibs->SetupAttachment(RootComponent);
	gibs->SetAutoActivate(false);

	enemyMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	enemyMesh->SetCollisionObjectType(ECC_PhysicsBody);
	//enemyMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	enemyMesh->SetEnableGravity(true);
	enemyMesh->SetSimulatePhysics(false);

	movement = GetCharacterMovement();
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

void AEnemyBase::hitEvent(float damage, float forceScaling)
{
	if (health > 0) {
		health -= damage;
		bloodGush->ActivateSystem();
	}
	if (isDead) // shoot as many times as you like to spawn as many gibs as you like I guess, should be funny
	{
		gibs->ActivateSystem();
		enemyMesh->DestroyComponent();
		destroyFace();

	}
	if (health <= 0.f && !isDead)
	{
		isDead = true;
		// Disable all collision on capsule
		//collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//collider->SetCollisionResponseToAllChannels(ECR_Ignore);
		if (!bIsRagdoll) {
			collider->DestroyComponent();
			enemyMesh->SetAllBodiesSimulatePhysics(true); // Start simulating physics to active ragdoll mode.
			enemyMesh->SetSimulatePhysics(true);
			enemyMesh->WakeAllRigidBodies();
			enemyMesh->bBlendPhysics = true;

			movement->StopMovementImmediately();
			movement->DisableMovement();
			movement->SetComponentTickEnabled(false);

			bIsRagdoll = true;
			enemyMesh->SetAllPhysicsLinearVelocity(FVector(0));

		}


		// Apply force from the attack.
		FVector lineFromPlayer = -GetActorRightVector();
		lineFromPlayer *= forceScaling;
		lineFromPlayer.Z *= 1.4f; // Add some extra force in the Z direction to simulate the "flying backwards and up" trope in movies when people get shot
		enemyMesh->AddImpulse(lineFromPlayer); // Head is still too heavy so this kinda doesn't work too well atm
		spawnMeat();
	}
}
void AEnemyBase::spawnMeat()
{
	if (Meat)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;

			FRotator rotator;

			FVector spawnLocation = this->enemyMesh->GetComponentLocation();
			FVector increaseZ(0, 0, 2);
			spawnLocation = spawnLocation + increaseZ;
			world->SpawnActor<AActor>(Meat, spawnLocation, rotator, spawnParams);
		}
	}

}