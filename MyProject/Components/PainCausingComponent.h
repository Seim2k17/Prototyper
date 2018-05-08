// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PainCausingTypes.h"
#include "PainCausingComponent.generated.h"

class UDamageType;
class USphereComponent;
class URadialForceComponent;

UCLASS( ClassGroup=(ImShaPrototype), meta=(BlueprintSpawnableComponent), showcategories = Trigger)
class MYPROJECT_API UPainCausingComponent : public UActorComponent
//wanted to visualize DamageRadius like in RadialForceComponent -> usesd the Component and the Radius from it instead
//class MYPROJECT_API UPainCausingComponent : public USceneComponent
{
	GENERATED_BODY()

/** METHODS */

public:	
	// Sets default values for this component's properties
	UPainCausingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Causes Pain in a special Interval*/
	void CauseTimerPain(AActor* OtherActor);

	void MakeRadialDamage(AActor* OtherActor);

	void SetRadius(float radius);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Damage overlapping actors if pain-causing. */
	virtual void PainTimer();

	/** damage overlapping actors if pain causing. */
	void CausePainTo(class AActor* Other);

	/**Initialize Variables */
	void InitializeVariables();



#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/** MEMBER */

public:	
	
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
	/**How Apply Damage to the Actor ?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		EPainCausingTypes PainCausingType;
	/**used ONLY IF RADIALDAMAGE is selected, Damage applied in certain Radius*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float AmountOfRadialDamage;
	/**used ONLY IF RADIALDAMAGE is selected, also Radius for the RadialForceComponent*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageRadius;
	/** Controller that gets credit for any damage caused by this Actor */
	UPROPERTY()
		class AController* DamageInstigator;

protected:
	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;

private:

	UPROPERTY()
	URadialForceComponent* RadialForceCompReference;

};
