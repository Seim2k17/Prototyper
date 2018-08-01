// Fill out your copyright notice in the Description page of Project Settings.

#include "MyClimbingActorBase.h"
#include "Components/BoxComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
AMyClimbingActorBase::AMyClimbingActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MainRoot"));
	RootComponent = SceneRoot;

	ClimbingTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ClimbActorTrigger"));
	ClimbingTrigger->SetupAttachment(SceneRoot);
	
	ClimbingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClimbableMesh"));
	ClimbingMesh->SetupAttachment(SceneRoot);

}

// Called when the game starts or when spawned
void AMyClimbingActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AMyClimbingActorBase::CanMoveUp() const
{
	return false;
}

bool AMyClimbingActorBase::CanMoveDown() const
{
	return false;
}

bool AMyClimbingActorBase::CanMoveRight() const
{
	return false;
}

bool AMyClimbingActorBase::CanMoveLeft() const
{
	return false;
}

void AMyClimbingActorBase::Interact()
{

}

