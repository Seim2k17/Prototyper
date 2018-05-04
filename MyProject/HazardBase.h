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
class USceneComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;
class ATargetPoint;
class ATriggeredObjects;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerFromHazardSignature,AActor*,Destination);

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

	/*DONE* damage overlapping actors if pain causing. */
	void CausePainTo(class AActor* Other);

	/*DONE* Damage overlapping actors if pain-causing. */
	virtual void PainTimer();

	/*DONE* Causes Pain in a special Interval*/
	void CauseTimerPain(AActor* OtherActor);

	/** Initialize Members / States*/
	void InitializeHazardVariables();

	/** Initialize Members / States*/
	void InitializeHazardVariablesBeginPlay();

	/*DONE* Play a VFX */
	void PlaySpecialEffect(AActor* OtherActor);

	/*DONE* Play a SFX */
	void PlaySoundEffect();

	/*DONE* Stop the PainCausingSound*/
	void StopSoundEffect();
	/**DONE*/
	void CheckTrigger();
	/**DONE*/
	void GetHazardImpact(AActor* OtherActor, FVector Impulse);
	/**DONE*/
	void TriggerEventTimer();
	/**DONE*/
	void ContinueTimer();



	/*Members UFUNCTIONS*/
	/*------------------*/
	//DONE
	UFUNCTION()
		void HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//DONE
	UFUNCTION()
		void HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//DONE
	UFUNCTION()
		void HandleTimedTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//DONE
	UFUNCTION()
		void HandleTimedTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//DONE
	UFUNCTION()
		void HandleSolidHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
	/* Member Variables - UPROPERTIES*/
	/*-------------------------------*/

public:


// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
// 	DONE	UStaticMeshComponent* HazardMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* HazardRoot;

	/*DONE* Whether volume currently causes damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainCausing;
	/*DONE* Damage done per second to actors in this volume when bPainCausing=true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float DamagePerSec;
	/*DONE* If pain causing, time between damage applications. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		float PainInterval;
	/*DONE* If bPainCausing == true, damage is dealt to the Pawn only once when entering the damage trigger (e.g. explosion) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		bool bPainIsOnce;
	/*DONE* Type of damage done */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pain Causing")
		TSubclassOf<UDamageType> DamageType;
	//....
	/*DONE* Is the Hazard destructible ? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		bool bDestructible;
	/*DONE* Is the Hazard destroyed after execution ? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		bool bDestroyActor;
	/*DONE* How much ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		FVector Impact;
	/*DONE* The Collision Preset you need to set to the colliding Components to deal damage (Trigger) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
		FName PresetNameTrigger;
	/*DONE* The Collision Preset you need to set to the colliding Components to deal damage (Solid)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
		FName PresetNameSolid;
	
	/*DONE* Special FX which plays at given location when Pawn enters Hazard*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* HazardFX;
	/*DONE* If true the FX is spawned at the Location the entered Pawn stands, if false a TargetPoint must be selected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		bool bFXSpawnsAtCharLocation;
	/*DONE* Offset to CharacterLocation where the FX is spawned*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		FVector FXOffset;
	/*DONE* TargetPoint at which the FX is spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		ATargetPoint* FXLocation;
	/*DONE* 2DSound FX played when Pawn enters Hazard and is damaged*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		USoundBase* HazardSFX;
	/*DONE* Whether the SoundFX stops playing immediatly when Pawn leaves the Damage Trigger (e.g. looping Sounds)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		bool bSoundStopsWhenLeaving;
	/*DONE* Reference to SFX above (to destroy it when needed*/
	UPROPERTY()
		UAudioComponent*  SFXReference;
	/*DONE* Controller that gets credit for any damage caused by this Actor */
	UPROPERTY()
		class AController* DamageInstigator;

	/** Sets the time in ms ([0..180k]) until the Event OnTriggerFromHazard is fired which can be implemented in Blueprint */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation", meta = (ClampMin = "0.0", ClampMax = "180000"))
		float TimeToTrigger;
	/** Sets the time between loops [0..10000] ms*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation", meta = (ClampMin = "0.0", ClampMax = "10000"))
		float TimeBetweenLoops;
	/** Repeating the Trigger ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation", meta = (ClampMin = "1", ClampMax = "100"))
		uint8 Loops;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activation")
		FName PresetNameTimedTrigger;
		
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnTriggerFromHazardSignature OnTriggerFromHazard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		//TSubclassOf<AActor> DestinationObject;
		ATriggeredObjects* DestinationObject;

protected:

	//DONE/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;

	FTimerHandle TimerHandle_TriggerEventTimer;

	FTimerHandle TimerHandle_TimerBtLoops;

private:
	UPROPERTY()
	uint8 StartLoops;
			
};
