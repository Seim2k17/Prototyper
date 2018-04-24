// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECustomTrigger.h"
#include "HazardBase.generated.h"


class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UShapeComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;
class ATargetPoint;

UCLASS()
class MYPROJECT_API AHazardBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazardBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/*Members NO-UFUNCTIONS*/
	/*---------------------*/
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** damage overlapping actors if pain causing. */
	void CausePainTo(class AActor* Other);

	/** Damage overlapping actors if pain-causing. */
	virtual void PainTimer();

	/** Causes Pain in a special Interval*/
	void CauseTimerPain(AActor* OtherActor);

	/** Initialize Members / States*/
	void InitializeHazardVariables();

	/** Play a VFX */
	void PlaySpecialEffect(AActor* OtherActor);

	/** Play a SFX */
	void PlaySoundEffect();

	/** Stop the PainCausingSound*/
	void StopSoundEffect();

	void CheckTrigger();

	void GetHazardImpact(AActor* OtherActor, FVector Impulse);



	/*Members UFUNCTIONS*/
	/*------------------*/
	
	UFUNCTION()
		void HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
		void HandleSolidHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

		
	/* Member Variables - UPROPERTIES*/
	/*-------------------------------*/

public:


// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
// 		UStaticMeshComponent* HazardMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* HazardRoot;

	/** Whether volume currently causes damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainCausing;
	/** Damage done per second to actors in this volume when bPainCausing=true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float DamagePerSec;
	/** If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float PainInterval;
	/** If bPainCausing == true, damage is dealt to the Pawn only once when entering the damage trigger (e.g. explosion) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainIsOnce;
	/** Type of damage done */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		TSubclassOf<UDamageType> DamageType;
	/** Is the Hazard destructible ? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		bool bDestructible;
	/** Is the Hazard destroyed after execution ? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		bool bDestroyActor;
	/** Is there a impact when enter the DamageTrigger ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		bool bGetImpact;
	/** How much ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		FVector Impact;
	/** The Collision Preset you need to set to the colliding Components to deal damage (Trigger) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
		FName PresetNameTrigger;
	/** The Collision Preset you need to set to the colliding Components to deal damage (Solid)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
		FName PresetNameSolid;
	
	/** Special FX which plays at given location when Pawn enters Hazard*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* HazardFX;
	/** If true the FX is spawned at the Location the entered Pawn stands, if false a TargetPoint must be selected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		bool bFXSpawnsAtCharLocation;
	/** Offset to CharacterLocation where the FX is spawned*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		FVector FXOffset;
	/** TargetPoint at which the FX is spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		ATargetPoint* FXLocation;
	/** 2DSound FX played when Pawn enters Hazard and is damaged*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* HazardSFX;
	/** Whether the SoundFX stops playing immediatly when Pawn leaves the Damage Trigger (e.g. looping Sounds)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		bool bSoundStopsWhenLeaving;
	/** Reference to SFX above (to destroy it when needed*/
	UPROPERTY()
		UAudioComponent*  SFXReference;
	/** Controller that gets credit for any damage caused by this Actor */
	UPROPERTY()
		class AController* DamageInstigator;
	
protected:

	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;

			
};
