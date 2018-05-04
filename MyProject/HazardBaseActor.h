// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredObjectBase.h"
#include "HazardBaseActor.generated.h"

class USceneComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;
class ATargetPoint;


UCLASS()
class MYPROJECT_API AHazardBaseActor : public ATriggeredObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazardBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Play a VFX */
	void PlaySpecialEffect(AActor* OtherActor);

	/** Play a SFX */
	void PlaySoundEffect();

	/** Stop the PainCausingSound*/
	void StopSoundEffect();

	void CheckTrigger();

	void GetHazardImpact(AActor* OtherActor, FVector Impulse);

	void InitializeValues();

	void CauseDamage(AActor* OtherActor);

	/*Members UFUNCTIONS*/
	/*------------------*/
	UFUNCTION()
		void HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
		void HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
		void HandleSolidHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* HazardRoot;

	/** The Collision Preset you need to set to the colliding Components to deal damage (Trigger) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
		FName PresetNameTrigger;

	/** The Collision Preset you need to set to the colliding Components to deal damage (Solid)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard")
		FName PresetNameSolid;

	/** UNIMPLEMENTED: Is the Hazard destructible ? Only relevant when HealthComponent attached , Broken if Health = 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		bool bDestructible;

	/** Is the Hazard destroyed after execution ? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		bool bDestroyActor;

	/** UNCONFIRMED* How much Impact in which direction ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
		FVector Impact;

	
	/** Special FX which plays at given location when Pawn collides with Hazard*/
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
};
