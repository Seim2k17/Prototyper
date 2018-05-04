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
 	AmountOfRadialDamage = 500.0f;
	DamageRadius = 500.0f;
	TriggerWhat = "Explode";

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}


void AExplosiveBarrel::TriggerSthing()
{
	ExplodeAtLocation(GetActorLocation());
}

void AExplosiveBarrel::ExplodeAtLocation(FVector FXLocation)
{
	const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("IMSHA.DebugObjects"));
	int32 DebugObjectsDrawing = CVar->GetInt();

	UE_LOG(LogTemp, Log, TEXT("BARREL EXPLODES"));
	if (ExplosiveFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosiveFX, FXLocation);
	}
	if (ExplosiveSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosiveSFX, FXLocation);
	}

	TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), AmountOfRadialDamage, GetActorLocation(), DamageRadius, DamageClassType, IgnoredActors, this);
 	if (DebugObjectsDrawing == 1)
 	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 12, FColor::Red, false, 1.5f, 0, 5.0f);
	}
}

