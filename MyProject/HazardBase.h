// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECustomTrigger.h"
#include "HazardBase.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UShapeComponent;

UCLASS()
class MYPROJECT_API AHazardBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazardBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void HandleMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void HandleTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void HandleMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void HandleTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

	//void ChangeTriggerShapeType(ECustomTriggerShapes ShapeType, T<UShapeComponent*> TriggerShape);

/* Membervariables*/

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UStaticMeshComponent* HazardMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UShapeComponent* HazardTrigger;
	/*	Select a Custom Shape for Trigger.
		Please reload Details-View by reselecting the Actor/BP in the WorldOutliner
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Trigger")
		ECustomTriggerShapes CustomTriggerShape;
// 	
// 	UPROPERTY()
// 		UBoxComponent BoxShapeTrigger;
// 	UPROPERTY()
// 		USphereComponent SphereShapeTrigger;
// 	UPROPERTY()
// 		UCapsuleComponent CapsuleShapeTrigger;


			
};
