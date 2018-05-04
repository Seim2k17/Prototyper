// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggeredObjects.h"
#include "ExplosiveBarrel.generated.h"

class USoundBase;
class UParticleSystem;
class UStaticMeshComponent;

UCLASS()
class MYPROJECT_API AExplosiveBarrel : public ATriggeredObjects
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void TriggerSthing() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* ExplosiveFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* ExplosiveSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float AmountOfRadialDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageRadius;
	//UPROPERTY(BlueprintAssignable, Category = "TriggeredObjects")
	UFUNCTION(BlueprintCallable, Category = "TriggeredObjects")
		void ExplodeAtLocation(FVector FXLocation);

	UPROPERTY()
	TSubclassOf<UDamageType> DamageClassType;
	
};
