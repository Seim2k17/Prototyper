// Fill out your copyright notice in the Description page of Project Settings.

#include "PainCausingComponent.h"

// Sets default values for this component's properties
UPainCausingComponent::UPainCausingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InitializeVariables();
	// ...
}

void UPainCausingComponent::InitializeVariables()
{
	DamagePerSec = 5.f;
	PainInterval = 0.05f;
	bPainIsOnce = false;

	DamageType = UDamageType::StaticClass();
}

// Called when the game starts
void UPainCausingComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPainCausingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPainCausingComponent::CausePainTo(class AActor* Other)
{
	if (DamagePerSec > 0.f)
	{
		TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass();
		Other->TakeDamage(DamagePerSec*PainInterval, FDamageEvent(DmgTypeClass), DamageInstigator, GetOwner());
	}

}

void UPainCausingComponent::PainTimer()
{
	TSet<AActor*> TouchingActors;
	GetOwner()->GetOverlappingActors(TouchingActors, APawn::StaticClass());
	for (AActor* const A : TouchingActors)
	{
		if (A && A->bCanBeDamaged && !A->IsPendingKill())
		{
			APawn* PawnA = Cast<APawn>(A);
			if (PawnA)
			{
				CausePainTo(A);
			}
		}
	}
	// Stop timer if nothing is overlapping us
	if (TouchingActors.Num() == 0)
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_PainTimer);
	}
}

void UPainCausingComponent::CauseTimerPain(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Log, TEXT("Entered Trigger"));
	if (OtherActor->bCanBeDamaged)
	{
		CausePainTo(OtherActor);
	}

	// Start timer if none is active
	if (!bPainIsOnce)
	{
		if (!GetOwner()->GetWorldTimerManager().IsTimerActive(TimerHandle_PainTimer))
		{
			GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_PainTimer, this, &UPainCausingComponent::PainTimer, PainInterval, true);
		}
	}
}
