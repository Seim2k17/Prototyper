// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PainCausingComponent.generated.h"

class UDamageType;

UCLASS( ClassGroup=(ImShaPrototype), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPainCausingComponent : public UActorComponent
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Damage overlapping actors if pain-causing. */
	virtual void PainTimer();

	/** damage overlapping actors if pain causing. */
	void CausePainTo(class AActor* Other);

	/**Initialize Variables */
	void InitializeVariables();

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
	/** Controller that gets credit for any damage caused by this Actor */
	UPROPERTY()
		class AController* DamageInstigator;

protected:
	/** Handle for efficient management of OnTimerTick timer */
	FTimerHandle TimerHandle_PainTimer;
		
	
};
