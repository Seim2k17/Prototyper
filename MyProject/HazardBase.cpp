// Fill out your copyright notice in the Description page of Project Settings.

#include "HazardBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHazardBase::AHazardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//"CreateDefault" is meant to be used inside constructor function only.
	HazardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HazardMesh"));
	RootComponent = HazardMesh;

	HazardMesh->OnComponentBeginOverlap.AddDynamic(this, &AHazardBase::HandleMeshBeginOverlap);
	HazardMesh->OnComponentEndOverlap.AddDynamic(this, &AHazardBase::HandleMeshEndOverlap);

	InitializeHazardVariables();
}

// Called when the game starts or when spawned
void AHazardBase::BeginPlay()
{
	Super::BeginPlay();

	if (bUsedCustomTrigger)
	{
		CheckCustomShapeTrigger();
	}
	
	
}

void AHazardBase::CausePainTo(class AActor* Other)
{
	if (DamagePerSec > 0.f)
	{
		TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass();
		Other->TakeDamage(DamagePerSec*PainInterval, FDamageEvent(DmgTypeClass), DamageInstigator, this);
	}

	if (bDestroyActor) this->Destroy();

}

void AHazardBase::PainTimer()
{
	if (bPainCausing)
	{
		TSet<AActor*> TouchingActors;
		GetOverlappingActors(TouchingActors, APawn::StaticClass());

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
			GetWorldTimerManager().ClearTimer(TimerHandle_PainTimer);
		}
	}

}

void AHazardBase::CauseTimerPain(AActor* OtherActor)
{
	//UE_LOG(LogTemp, Log, TEXT("Entered Trigger"));
	if (bPainCausing && OtherActor->bCanBeDamaged)
	{
		CausePainTo(OtherActor);
	}

	// Start timer if none is active
	if (!bPainIsOnce)
	{
		if (!GetWorldTimerManager().IsTimerActive(TimerHandle_PainTimer))
		{
			GetWorldTimerManager().SetTimer(TimerHandle_PainTimer, this, &AHazardBase::PainTimer, PainInterval, true);
		}
	}
}

void AHazardBase::InitializeHazardVariables()
{

	bUsedCustomTrigger = true;
	bPainCausing = true;
	DamagePerSec = 5.f;
	PainInterval = 0.05f;
	bPainIsOnce = false;
	bDestroyActor = false;
	bDestructible = false;
	DamageType = UDamageType::StaticClass();
	CollisionPresetName = "Trigger";
	
}

void AHazardBase::PlaySpecialEffect()
{
	if (HazardFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HazardFX, GetActorLocation());
	}
}

void AHazardBase::PlaySoundEffect()
{
	if (HazardSFX != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("PLAY SFX !"));
		SFXReference = UGameplayStatics::SpawnSound2D(this, HazardSFX);
	}

}

void AHazardBase::StopSoundEffect()
{
	UE_LOG(LogTemp, Log, TEXT("SFX DIE !!!"));
	if (SFXReference)
	{
		SFXReference->Stop();
	}

}

void AHazardBase::CheckCustomShapeTrigger()
{
	TArray<USceneComponent*> Comps;
	HazardMesh->GetChildrenComponents(true, Comps);

	uint8 shapeCount = 0;

	if (Comps.Num() > 0)
	{
		for(USceneComponent* el : Comps)
		{
			if (Cast<UPrimitiveComponent>(el))
			{
				UPrimitiveComponent* PrimEl = Cast<UPrimitiveComponent>(el);
				FName ColName = PrimEl->GetCollisionProfileName();
				if (ColName == CollisionPresetName)
				{
					++shapeCount;
					PrimEl->OnComponentBeginOverlap.RemoveAll(this);
					PrimEl->OnComponentEndOverlap.RemoveAll(this);
					PrimEl->OnComponentBeginOverlap.AddDynamic(this, &AHazardBase::HandleTriggerBeginOverlap);
					PrimEl->OnComponentEndOverlap.AddDynamic(this, &AHazardBase::HandleTriggerEndOverlap);
					UE_LOG(LogTemp, Log, TEXT("TriggerElements: %d %s"), shapeCount, *ColName.ToString());
				}
			}
		}
	}
}

void AHazardBase::HandleMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("MeshCollision entered"));
	if (!bUsedCustomTrigger)
	{
		CauseTimerPain(OtherActor);
		if (bPainIsOnce)
		{
			PlaySpecialEffect();
		}
		PlaySoundEffect();
	}

}

void AHazardBase::HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("CustomTrigger entered"));
	if (bUsedCustomTrigger)
	{
		CauseTimerPain(OtherActor);
		if (bPainIsOnce)
		{
			PlaySpecialEffect();
		}
		PlaySoundEffect();
	}

}

void AHazardBase::HandleMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap out (MeshCollision)"));
	StopSoundEffect();
}

void AHazardBase::HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap out (Custom Shape Collision)"));
	StopSoundEffect();
}


// Called every frame
void AHazardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

