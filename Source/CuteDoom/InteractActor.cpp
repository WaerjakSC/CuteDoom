// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActor.h"

// Sets default values
AInteractActor::AInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// TODO: This should be filled with whatever we want the object to do in-game.
// A blueprint version can also be made, and you can use one or both at will.
void AInteractActor::Activate_Implementation(){}