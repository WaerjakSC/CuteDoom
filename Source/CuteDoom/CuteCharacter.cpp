// Fill out your copyright notice in the Description page of Project Settings.

#include "CuteCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "InteractActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"
#include "Shotgun.h"

// Sets default values
ACuteCharacter::ACuteCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to
	// improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	ArmsMesh->SetOnlyOwnerSee(true);
	ArmsMesh->SetupAttachment(FirstPersonCameraComponent);
	ArmsMesh->bCastDynamicShadow = false;
	//ArmsMesh->CastShadow = false;
	ArmsMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	ArmsMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FPGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPGun"));
	FPGun->SetOnlyOwnerSee(true); // only the owning player will see this mesh
	FPGun->bCastDynamicShadow = false;
	FPGun->CastShadow = false;
	FPGun->SetupAttachment(ArmsMesh, TEXT("GripPoint"));
	// Note: This is invalid, GripPoint will need to be changed to whatever we name the gun socket in our version.
	FPGun->SetupAttachment(RootComponent);

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
}

// Called when the game starts or when spawned
void ACuteCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FPGun->AttachToComponent(ArmsMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
	//                         TEXT("GripPoint"));
	// Set the Kick WeaponStats UObject from the given blueprint
	KickWeapon = NewObject<UWeapon>(this, KickBP->GetFName(), RF_NoFlags, KickBP.GetDefaultObject());
	Pistol = NewObject<UWeapon>(this, PistolBP->GetFName(), RF_NoFlags, PistolBP.GetDefaultObject());
	Shotgun = NewObject<UWeapon>(this, ShotgunBP->GetFName(), RF_NoFlags, ShotgunBP.GetDefaultObject());
	SetWeapon(CurrentWeapon);
}

// Called every frame
void ACuteCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACuteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACuteCharacter::Shoot);

	// Bind kick event
	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &ACuteCharacter::Kick);

	// Bind interact event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACuteCharacter::Interact);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACuteCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACuteCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACuteCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACuteCharacter::LookUpAtRate);
}

void ACuteCharacter::Shoot()
{
	Attack(CurWeapon);
}

void ACuteCharacter::Kick()
{
	Attack(KickWeapon);
}

void ACuteCharacter::Attack(const UWeapon* Weapon)
{
	FHitResult TraceResult(ForceInit);
	FCollisionQueryParams RV_TraceParams =
		FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	FVector TraceEnd{GetActorForwardVector() * Weapon->GetRange()};
	const UShotgun* ShotgunWeapon{Cast<UShotgun>(Weapon)};
	for (int i{0}; i < Weapon->GetHitsPerAttack(); i++)
	{
		if (ShotgunWeapon)
		{
			const float Spread{FMath::FRandRange(-ShotgunWeapon->GetSpread(), ShotgunWeapon->GetSpread())};
			TraceEnd = TraceEnd.RotateAngleAxis(Spread, GetActorUpVector());
		}
		if (DoTrace(TraceResult, RV_TraceParams, TraceEnd))
		{
			AActor* DamagedActor = TraceResult.GetActor();

			UPrimitiveComponent* DamagedComponent = TraceResult.GetComponent();
			if (bDebugMode)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.f,
					FColor::Red,
					FString::Printf(TEXT("Hit Target: x: %s"), *DamagedActor->GetName()));
			}

			// If we hit an actor
			if ((DamagedActor != nullptr) &&
				(DamagedComponent != nullptr))
			{
				FVector HitDirection{GetActorLocation() - DamagedActor->GetActorLocation()};

				// with a component that is simulating physics, apply an impulse
				if (AEnemyBase* Enemy = Cast<AEnemyBase>(DamagedActor))
				{
					FPointDamageEvent DamageEvent{Weapon->GetDamage(), TraceResult, HitDirection, nullptr};
					Enemy->HitEvent(this, DamageEvent, Weapon);
				}
			}
		}
	}
}


bool ACuteCharacter::DoTrace(FHitResult& RV_Hit,
                             FCollisionQueryParams& RV_TraceParams, const FVector TraceEnd) const
{
	if (Controller == nullptr) // access the controller, make sure we have one
	{
		return false;
	}

	// get the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	const FVector Start{CameraLoc + FVector(0, 0, -40.f)};

	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = true;

	//  do the line trace
	const bool DidTrace{
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit, // result
			Start, // start
			TraceEnd, // end
			ECC_Pawn, // collision channel -- Set this to something else
			RV_TraceParams)
	};
	if (bDebugMode)
	{
		DrawDebugLine(GetWorld(), Start, TraceEnd, FColor::Red, false, 1.f);
	}


	return DidTrace;
}

void ACuteCharacter::Interact()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (auto Actor : OverlappingActors)
	{
		if (Actor->Implements<UInteractable>()) // Check if the actor implements our Interactable interface
		{
			// It does, so we execute the activate function. This may be a different function based on the actor.
			IInteractable::Execute_Activate(Actor);
			// Actor should always be the first variable, after which you can add any extra parameters the function asks for.
		}
	}
}

float ACuteCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void ACuteCharacter::SetWeapon(const ESelectedWeapon NewWeapon)
{
	CurrentWeapon = NewWeapon;
	switch (CurrentWeapon)
	{
	case ESelectedWeapon::Pistol:
		CurWeapon = Pistol;
		break;
	case ESelectedWeapon::Shotgun:
		CurWeapon = Shotgun;
		break;
	default:
		break;
	}
}

void ACuteCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		GetCharacterMovement()->bForceMaxAccel = true;
	}
	GetCharacterMovement()->bForceMaxAccel = false;
}

void ACuteCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		GetCharacterMovement()->bForceMaxAccel = true;
	}
	GetCharacterMovement()->bForceMaxAccel = false;
}

void ACuteCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACuteCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
