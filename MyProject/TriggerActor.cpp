// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerActor.h"
#include "PainCausingTypes.h"
#include "Components/PainCausingComponent.h"


// Sets default values
ATriggerActor::ATriggerActor()
{
 	//"CreateDefault" is meant to be used inside constructor function only.
	TriggerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HazardRoot"));
	RootComponent = TriggerRoot;

	TimeToTrigger = 0.f;
	PresetNameTimedTrigger = "HazardTimedTrigger";
	Loops = 1;
}

// Called when the game starts or when spawned
void ATriggerActor::BeginPlay()
{
	Super::BeginPlay();
	CheckTrigger();
	StartLoops = Loops;
}

void ATriggerActor::CheckTrigger()
{
	TArray<USceneComponent*> Comps;
	TriggerRoot->GetChildrenComponents(true, Comps);

	if (Comps.Num() > 0)
	{
		for (USceneComponent* el : Comps)
		{
			if (Cast<UPrimitiveComponent>(el))
			{
				UPrimitiveComponent* PrimEl = Cast<UPrimitiveComponent>(el);
				FName ColName = PrimEl->GetCollisionProfileName();
				if (ColName == PresetNameTimedTrigger)
				{
					PrimEl->OnComponentBeginOverlap.RemoveAll(this);
					PrimEl->OnComponentEndOverlap.RemoveAll(this);
					PrimEl->OnComponentBeginOverlap.AddDynamic(this, &ATriggerActor::HandleTimedTriggerBeginOverlap);
					PrimEl->OnComponentEndOverlap.AddDynamic(this, &ATriggerActor::HandleTimedTriggerEndOverlap);
				}
			}
		}
	}
}

void ATriggerActor::TriggerEventTimer()
{

	if (Loops > 0)
	{

		UE_LOG(LogTemp, Warning, TEXT("AN EVENT OCCURED !: %s .time"), *FString::FromInt(Loops));
		//OnTriggerFromHazard.Broadcast(DestinationObject);
		if (DestinationObject)
		{
			ApplyRadialDamage(DestinationObject);
			DestinationObject->TriggerSomethingOverlapIn(DestinationObject->TriggeredObjectName);
		}
		--Loops;
		if (TimeBetweenLoops > 0.0f)
		{
			GetWorldTimerManager().PauseTimer(TimerHandle_TriggerEventTimer);
			GetWorldTimerManager().SetTimer(TimerHandle_TimerBtLoops, this, &ATriggerActor::ContinueTimer, TimeBetweenLoops / 1000, false);
		}

		return;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_TriggerEventTimer);
		Loops = StartLoops;
	}


}

void ATriggerActor::ContinueTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimerBtLoops);
	GetWorldTimerManager().UnPauseTimer(TimerHandle_TriggerEventTimer);
	UE_LOG(LogTemp, Log, TEXT("HandleTimedTrigger_EventTimer, Loops: %s, StartLoops: %s"), *FString::FromInt(Loops), *FString::FromInt(StartLoops));
}

void ATriggerActor::HandleTimedTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	/** DebugCheck TriggerEvent*/
	bool bIsLooping = false;
	if (Loops > 0) bIsLooping = true;
	GetWorldTimerManager().SetTimer(TimerHandle_TriggerEventTimer, this, &ATriggerActor::TriggerEventTimer, (TimeToTrigger / 1000), bIsLooping);
}

void ATriggerActor::HandleTimedTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (DestinationObject) DestinationObject->TriggerSomethingOverlapOut(DestinationObject->TriggeredObjectName);
}


void ATriggerActor::ApplyRadialDamage(AActor* OtherActor)
{
	TArray<UPainCausingComponent*> CompArray;
	OtherActor->GetComponents<UPainCausingComponent>(CompArray);
	if (CompArray.Num() > 0)
	{
		//take first Hit !
		if (CompArray[0]->PainCausingType == EPainCausingTypes::RadialDamage)
		{
			UE_LOG(LogTemp, Log, TEXT("RADIAL_BAEMM"));
			CompArray[0]->MakeRadialDamage(OtherActor);
		}
	}
}



