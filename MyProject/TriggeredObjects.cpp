// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggeredObjects.h"


// Sets default values
ATriggeredObjects::ATriggeredObjects()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void ATriggeredObjects::BeginPlay()
{
	Super::BeginPlay();

}

void ATriggeredObjects::TriggerSthing()
{

}