// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class USoundBase;
class UParticleSystem;
class UStaticMeshComponent;

UCLASS()
class MYPROJECT_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* BarrelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* ExplosiveFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* ExplosiveSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float AmountOfRadialDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageRadius;

	UFUNCTION(BlueprintCallable, Category = "Barrel")
	void Explode();

	TSubclassOf<UDamageType> DamageClassType;
	
};
