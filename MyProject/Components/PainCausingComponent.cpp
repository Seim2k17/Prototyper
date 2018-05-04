// Fill out your copyright notice in the Description page of Project Settings.

#include "PainCausingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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
	PainCausingType = EPainCausingTypes::JustPain;
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
	const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("IMSHA.DebugObjects"));
	int32 DebugObjectsDrawing = CVar->GetInt();

	if (DamagePerSec > 0.f)
	{
		TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass();
		Other->TakeDamage(DamagePerSec*PainInterval, FDamageEvent(DmgTypeClass), DamageInstigator, GetOwner());
		if (DebugObjectsDrawing == 2) DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), TEXT("OUCH"), NULL, FColor::Red, 2.0f);
	}
	//apply other damage types ? PointDamage/RadialDamage -> Once ? -> parameter siehe bp

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

void UPainCausingComponent::MakeRadialDamage(AActor* OtherActor)
{
	const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("IMSHA.DebugObjects"));
	int32 DebugObjectsDrawing = CVar->GetInt();

	TArray<AActor*> IgnoredActors;
	AActor* MyOwner = GetOwner();
	UGameplayStatics::ApplyRadialDamage(GetWorld(), AmountOfRadialDamage, MyOwner->GetActorLocation(), DamageRadius, DamageType, IgnoredActors, MyOwner);
	if (DebugObjectsDrawing == 1) DrawDebugSphere(GetWorld(), MyOwner->GetActorLocation(), DamageRadius, 12, FColor::Red, false, 1.5f, 0, 1.0f);
}
