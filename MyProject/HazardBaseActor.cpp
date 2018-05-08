// Fill out your copyright notice in the Description page of Project Settings.
#include "HazardBaseActor.h"
#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PainCausingComponent.h"
#include "Components/HazardRadialForceComponent.h"


// Sets default values
AHazardBaseActor::AHazardBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//"CreateDefault" is meant to be used inside constructor function only.
	HazardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HazardRoot"));
	RootComponent = HazardRoot;

	RadialForce = CreateDefaultSubobject<UHazardRadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(HazardRoot);
	
	InitializeValues();
}

void AHazardBaseActor::InitializeValues()
{
	bDestroyActor = false;
	bDestructible = false;
	bSoundStopsWhenLeaving = false;
	bFXSpawnsAtCharLocation = false;

	PresetNameTrigger = "HazardDamageTrigger";
	PresetNameSolid = "HazardDamageSolid";

	//no Pulse at Spawn
	RadialForce->ImpulseStrength = 0.0f;
	//does not allow RadialForce to Tick in the firstplace
	RadialForce->bIsActive = false;
	RadialForce->bImpulseVelChange = true;
	RadialForce->bIgnoreOwningActor = true;
}

// Called when the game starts or when spawned
void AHazardBaseActor::BeginPlay()
{
	Super::BeginPlay();
	CheckTrigger();
}

// Called every frame
void AHazardBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHazardBaseActor::CheckTrigger()
{
	TArray<USceneComponent*> Comps;
	HazardRoot->GetChildrenComponents(true, Comps);

	if (Comps.Num() > 0)
	{
		for (USceneComponent* el : Comps)
		{
			if (Cast<UPrimitiveComponent>(el))
			{
				UPrimitiveComponent* PrimEl = Cast<UPrimitiveComponent>(el);
				FName ColName = PrimEl->GetCollisionProfileName();

				if (ColName == PresetNameTrigger)
				{
					PrimEl->OnComponentBeginOverlap.RemoveAll(this);
					PrimEl->OnComponentEndOverlap.RemoveAll(this);
					PrimEl->OnComponentBeginOverlap.AddDynamic(this, &AHazardBaseActor::HandleTriggerBeginOverlap);
					PrimEl->OnComponentEndOverlap.AddDynamic(this, &AHazardBaseActor::HandleTriggerEndOverlap);
				}
				if (ColName == PresetNameSolid)
				{
					PrimEl->OnComponentHit.RemoveAll(this);
					PrimEl->OnComponentHit.AddDynamic(this, &AHazardBaseActor::HandleSolidHit);

				}
			}
		}
	}
}

void AHazardBaseActor::PlaySpecialEffect(AActor* OtherActor)
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

void AHazardBaseActor::PlaySoundEffect()
{
	if (HazardSFX != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("PLAY SFX !"));
		SFXReference = UGameplayStatics::SpawnSound2D(this, HazardSFX);
	}
}

void AHazardBaseActor::StopSoundEffect()
{
	UE_LOG(LogTemp, Log, TEXT("SFX DIE !!!"));
	if (SFXReference && bSoundStopsWhenLeaving)
	{
		SFXReference->Stop();
	}
}


void AHazardBaseActor::GetHazardImpact(AActor* OtherActor, FVector Impulse)
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

void AHazardBaseActor::HandleTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("CustomTrigger entered"));
	CauseDamage(OtherActor);
	PlaySpecialEffect(OtherActor);
	PlaySoundEffect();

}

void AHazardBaseActor::HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap out (Custom Shape Collision)"));
	if (bSoundStopsWhenLeaving) StopSoundEffect();
}

void AHazardBaseActor::HandleSolidHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Solid Hit."));
	GetHazardImpact(OtherActor, Impact);
	CauseDamage(OtherActor);
	PlaySpecialEffect(OtherActor);
	PlaySoundEffect();

}

void AHazardBaseActor::CauseDamage(AActor* OtherActor)
{
	TArray<UPainCausingComponent*> CompArray;
	GetComponents<UPainCausingComponent>(CompArray);
	if (CompArray.Num() > 0)
	{
		//take first Hit !
		CompArray[0]->CauseTimerPain(OtherActor);
	}
}

