// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveBarrel.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	RootComponent = BarrelMesh;

	AmountOfRadialDamage = 500.0f;
	DamageRadius = 500.0f;

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::Explode()
{
	UE_LOG(LogTemp, Log, TEXT("BARREL EXPLODES"));
	if (ExplosiveFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosiveFX, GetActorLocation());
	}
	if (ExplosiveSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosiveSFX, GetActorLocation());
	}

	TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), AmountOfRadialDamage, GetActorLocation(), DamageRadius, DamageClassType, IgnoredActors, this);
	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 12, FColor::Red, false, 1.5f, 0, 5.0f);
	
	
}

