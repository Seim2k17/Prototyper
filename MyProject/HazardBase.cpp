// Fill out your copyright notice in the Description page of Project Settings.

#include "HazardBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AHazardBase::AHazardBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//"CreateDefault" is meant to be used inside constructor function only.
	HazardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HazardMesh"));
	RootComponent = HazardMesh;
}

// Called when the game starts or when spawned
void AHazardBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHazardBase::HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void AHazardBase::HandleTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void AHazardBase::HandleMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AHazardBase::HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AHazardBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (HazardTrigger) HazardTrigger->DestroyComponent();
	
	switch (CustomTriggerShape)
	{
		case ECustomTriggerShapes::none:
			UE_LOG(LogTemp, Log, TEXT("Do NONEThings"));
			break;
		case ECustomTriggerShapes::BoxTrigger:
			UE_LOG(LogTemp,Log, TEXT("Do BOXThings"));
			HazardTrigger = Cast<UBoxComponent>(NewObject<UBoxComponent>(this));
			break;
		case ECustomTriggerShapes::SphereTrigger:
			UE_LOG(LogTemp,Log, TEXT("Do SPHEREThings"));
			HazardTrigger = Cast<USphereComponent>(NewObject<USphereComponent>(this));
			break;
		case ECustomTriggerShapes::CapsuleTrigger:
			UE_LOG(LogTemp,Log, TEXT("Do CAPSULEThings"));
			HazardTrigger = Cast<UCapsuleComponent>(NewObject<UCapsuleComponent>(this));
			break;
	}

	if (HazardTrigger) HazardTrigger->SetupAttachment(HazardMesh);

	//HazardTrigger->

	Super::PostEditChangeProperty(PropertyChangedEvent);
}


//tried to parametrisize a function with undefined Types (Triggershapes are childclasses from UShapeComponent)
// void AHazardBase::ChangeTriggerShapeType(ECustomTriggerShapes ShapeType, UShapeComponent TriggerShape)
// {
// // 	if (HazardTrigger) HazardTrigger->DestroyComponent();
// // 
// // 	HazardTrigger = Cast<TriggerShape::StaticClass()>(NewObject<TriggerShape::StaticClass()>(this));
// // 	HazardTrigger->SetupAttachment(HazardMesh);
// // 	CustomTriggerShape = ShapeType;
// 
// }

// Called every frame
void AHazardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

