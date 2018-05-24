// Fill out your copyright notice in the Description page of Project Settings.
/**OUTDATED CLASS*/


#include "HazardBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TriggeredObjects.h"


// Sets default values
AHazardBase::AHazardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//"CreateDefault" is meant to be used inside constructor function only.
	HazardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HazardRoot"));
	RootComponent = HazardRoot;

	InitializeHazardVariables();
}

void AHazardBase::InitializeHazardVariables()
{

	bPainCausing = true;
	DamagePerSec = 5.f;
	PainInterval = 0.05f;
	bPainIsOnce = false;
	bDestroyActor = false;
	bDestructible = false;
	bSoundStopsWhenLeaving = false;
	bFXSpawnsAtCharLocation = false;
		
	DamageType = UDamageType::StaticClass();
	PresetNameTrigger = "HazardDamageTrigger";
	PresetNameSolid = "HazardDamageSolid";
	TimeToTrigger = 0.f;

	PresetNameTimedTrigger = "HazardTimedTrigger";

	Loops = 1;

}

void AHazardBase::InitializeHazardVariablesBeginPlay()
{
	StartLoops = Loops;
}

// Called when the game starts or when spawned
void AHazardBase::BeginPlay()
{
	Super::BeginPlay();
	CheckTrigger();
	InitializeHazardVariablesBeginPlay();

}

// Called every frame
void AHazardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHazardBase::CausePainTo(class AActor* Other)
{
	//DONE-START
	if (DamagePerSec > 0.f)
	{
		TSubclassOf<UDamageType> DmgTypeClass = DamageType ? *DamageType : UDamageType::StaticClass();
		Other->TakeDamage(DamagePerSec*PainInterval, FDamageEvent(DmgTypeClass), DamageInstigator, this);
	}
	//DONE-END

	if (bDestroyActor) this->Destroy(); //move to HazardBase (if Health=0 and bDestroy -> destroy)

}

//DONE
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

//DONE
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

//done
void AHazardBase::PlaySpecialEffect(AActor* OtherActor)
{
	FVector FXTmpLocation;
	bool bTestFailed;

	if (bFXSpawnsAtCharLocation)
	{

		FXTmpLocation = OtherActor->GetActorLocation() + FXOffset;
		if (HazardFX == nullptr) bTestFailed = true;
	}
	else
	{
		if ((FXLocation == nullptr) || (HazardFX == nullptr)) bTestFailed = true;
		else FXTmpLocation = FXLocation->GetActorLocation();
		
	}
	
	if (bTestFailed)
	{
		//Warning ?
		//UE_LOG(LogTemp, Warning, TEXT("FX is not played. Please check if a Location and a Particle-system are set."));
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HazardFX, FXTmpLocation);
	}
}

//done
void AHazardBase::PlaySoundEffect()
{
	if (HazardSFX != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("PLAY SFX !"));
		SFXReference = UGameplayStatics::SpawnSound2D(this, HazardSFX);
	}

}
//done
void AHazardBase::StopSoundEffect()
{
	UE_LOG(LogTemp, Log, TEXT("SFX DIE !!!"));
	if (SFXReference && bSoundStopsWhenLeaving)
	{
		SFXReference->Stop();
	}

}

//done
void AHazardBase::CheckTrigger()
{
	TArray<USceneComponent*> Comps;
	HazardRoot->GetChildrenComponents(true, Comps);

	if (Comps.Num() > 0)
	{
		for(USceneComponent* el : Comps)
		{
			if (Cast<UPrimitiveComponent>(el))
			{
				UPrimitiveComponent* PrimEl = Cast<UPrimitiveComponent>(el);
				FName ColName = PrimEl->GetCollisionProfileName();
				
// 				switch (ColName)
// 				{
// 				case PresetNameTrigger:
// 				case PresetNameSolid:
// 				}
				
				if (ColName == PresetNameTrigger)
				{
					PrimEl->OnComponentBeginOverlap.RemoveAll(this);
					PrimEl->OnComponentEndOverlap.RemoveAll(this);
					PrimEl->OnComponentBeginOverlap.AddDynamic(this, &AHazardBase::HandleTriggerBeginOverlap);
					PrimEl->OnComponentEndOverlap.AddDynamic(this, &AHazardBase::HandleTriggerEndOverlap);
				}
				if (ColName == PresetNameSolid)
				{
					PrimEl->OnComponentHit.RemoveAll(this);
					PrimEl->OnComponentHit.AddDynamic(this, &AHazardBase::HandleSolidHit);
					
				}
				if (ColName == PresetNameTimedTrigger)
				{
					PrimEl->OnComponentBeginOverlap.RemoveAll(this);
					PrimEl->OnComponentEndOverlap.RemoveAll(this);
					PrimEl->OnComponentBeginOverlap.AddDynamic(this, &AHazardBase::HandleTimedTriggerBeginOverlap);
					PrimEl->OnComponentEndOverlap.AddDynamic(this, &AHazardBase::HandleTimedTriggerEndOverlap);

				}
			}
		}
	}
}


void AHazardBase::GetHazardImpact(AActor* OtherActor, FVector Impulse)
{
	if (Cast<ACharacterBase>(OtherActor))
		{
			UE_LOG(LogTemp, Log, TEXT("It´s a Pawn !"));
			//Impact only possible on character
			ACharacterBase* MyChar = Cast<ACharacterBase>(OtherActor);
			//
			//MyChar->GetCapsuleComponent()->AddImpulse(Impulse);

			//take a look in the ThirdPersonCharacter BP -> 1:1
			MyChar->GetCharacterMovement()->DisableMovement();
			MyChar->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MyChar->GetMesh()->SetAllBodiesSimulatePhysics(true);
			UE_LOG(LogTemp, Log, TEXT("Impulse: %s"), *Impulse.ToString());
			//"verfeinern" was passiert (use noPhysics)
			MyChar->GetMesh()->AddImpulse(Impulse);

		}
}

void AHazardBase::TriggerEventTimer()
{
	
	if (Loops > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AN EVENT OCCURED !: %s .time"),*FString::FromInt(Loops));
		OnTriggerFromHazard.Broadcast(DestinationObject);
		if (DestinationObject) DestinationObject->TriggerSthing();
		--Loops;
		if (TimeBetweenLoops > 0.0f)
		{
			GetWorldTimerManager().PauseTimer(TimerHandle_TriggerEventTimer);
			GetWorldTimerManager().SetTimer(TimerHandle_TimerBtLoops, this, &AHazardBase::ContinueTimer, TimeBetweenLoops / 1000, false);
		}
		
		return;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_TriggerEventTimer);
		Loops = StartLoops;
	}

}

void AHazardBase::ContinueTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimerBtLoops);
	GetWorldTimerManager().UnPauseTimer(TimerHandle_TriggerEventTimer);
	UE_LOG(LogTemp, Log, TEXT("HandleTimedTrigger_EventTimer, Loops: %s, StartLoops: %s"), *FString::FromInt(Loops), *FString::FromInt(StartLoops));
}

void AHazardBase::HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (bPainCausing)
	{
		UE_LOG(LogTemp, Log, TEXT("CustomTrigger entered"));
		CauseTimerPain(OtherActor);
		PlaySpecialEffect(OtherActor);
		PlaySoundEffect();
	}
}

void AHazardBase::HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap out (Custom Shape Collision)"));
	if (bSoundStopsWhenLeaving) StopSoundEffect();
}


void AHazardBase::HandleSolidHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Solid Hit."));
	GetHazardImpact(OtherActor, Impact);
	CauseTimerPain(OtherActor);
	PlaySpecialEffect(OtherActor);
	PlaySoundEffect();
}

void AHazardBase::HandleTimedTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	/** DebugCheck TriggerEvent*/
	bool bIsLooping = false;
	if (Loops > 0) bIsLooping = true;
	GetWorldTimerManager().SetTimer(TimerHandle_TriggerEventTimer, this, &AHazardBase::TriggerEventTimer, (TimeToTrigger / 1000), bIsLooping);
}

void AHazardBase::HandleTimedTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}