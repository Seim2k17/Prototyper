// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECustomTrigger.h"
#include "HazardBase.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UShapeComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;

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
	void PlaySpecialEffect();

	/** Play a SFX */
	void PlaySoundEffect();

	/** Stop the PainCausingSound*/
	void StopSoundEffect();

	void CheckCustomShapeTrigger();



	/*Members UFUNCTIONS*/
	/*------------------*/
	UFUNCTION()
		void HandleMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void HandleMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	//void ChangeTriggerShapeType(ECustomTriggerShapes ShapeType, T<UShapeComponent*> TriggerShape);

	/* Member Variables - UPROPERTIES*/
	/*-------------------------------*/

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UStaticMeshComponent* HazardMesh;

	/* if bUsedCustomTrigger then a custom CustomTrigger is used */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Trigger")
		bool bUsedCustomTrigger;
	/*	Select a Custom Shape for Trigger.
		Please reload Details-View by reselecting the Actor/BP in the WorldOutliner
	*/
	/** Whether volume currently causes damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainCausing;
	/** Damage done per second to actors in this volume when bPainCausing=true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float DamagePerSec;
	/** If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float PainInterval;
	/** if bPainCausing, causes pain only once (e.g. explosion*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainIsOnce;
	/** is the Hazard destructible ? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bDestructible;
	/** is the Hazard destroyed after execution ? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bDestroyActor;
	/** Type of damage done */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		TSubclassOf<UDamageType> DamageType;
	/** Controller that gets credit for any damage caused by this Actor */
	UPROPERTY()
		class AController* DamageInstigator;
	/** Special FX which plays when Actor enters Hazard*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* HazardFX;
	/** Sound FX when damaged*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* HazardSFX;
	/** Reference to SFX above (to destroy it when needed*/
	UPROPERTY()
		UAudioComponent*  SFXReference;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Trigger")
		FName CollisionPresetName;

protected:

	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;

			
};
