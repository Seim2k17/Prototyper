// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredObjectBase.h"
#include "TriggeredObjectNames.h"
#include "TriggerActor.generated.h"

UCLASS()

//Trigger for Hazards to trigger
class MYPROJECT_API ATriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerActor();

	UFUNCTION()
		void HandleTimedTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void HandleTimedTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckTrigger();

	void TriggerEventTimer();

	void ContinueTimer();

	void ApplyRadialDamage(AActor* OtherActor);


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* TriggerRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activation")
		FName PresetNameTimedTrigger;

	/** Sets the time in ms ([0..180k]) until the Event OnTriggerFromHazard is fired which can be implemented in Blueprint */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Activation", meta = (ClampMin = "0.0", ClampMax = "180000"))
		float TimeToTrigger;

	/** Sets the time between loops [0..10000] ms*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Activation", meta = (ClampMin = "0.0", ClampMax = "10000"))
		float TimeBetweenLoops;

	/** Repeating the Trigger ?*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Activation", meta = (ClampMin = "1", ClampMax = "100"))
		uint8 Loops;

	/**Name of the Trigger / CPP version GD likable ?*/
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
// 		ETriggerNames TriggerName;

// 	UPROPERTY(BlueprintAssignable, Category = "Events")
// 		FOnTriggerFromHazardSignature OnTriggerFromHazard;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Activation")
		ATriggeredObjectBase* DestinationObject;

	
protected:

	FTimerHandle TimerHandle_TriggerEventTimer;

	FTimerHandle TimerHandle_TimerBtLoops;


private:
	UPROPERTY()
		uint8 StartLoops;


};
