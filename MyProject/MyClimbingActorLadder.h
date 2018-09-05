// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyClimbingActorBase.h"
#include "MyClimbingActorLadder.generated.h"

class UChildActorComponent;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyClimbingActorLadder : public AMyClimbingActorBase
{
	GENERATED_BODY()

		
public:

		AMyClimbingActorLadder();

		virtual void Interact() override;

		void CheckEnterPositionReached();

		void CheckExitPositionReached();

		void BeginPlay() override;

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		UFUNCTION()
		void OnHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
			bool bFromSweep, const FHitResult & SweepResult);

		UFUNCTION()
		void OnHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

//** Member **//

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float DestinationTraceTreshhold;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* TargetPointExitTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* EnterTopPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float DistanceHandsToTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float DistanceBottomExit;

	
private:

	bool bCharacterWalksToEnterDestination;
	bool bCharacterNearExitPosition;
	float DestinationToUpTrace;
	float DestinationToDownTrace;

	FVector CurrentLadderEnterPosition;

	//Tracees for enter and exit ladder
	FCollisionQueryParams QueryHitParams;
	FHitResult TraceHit;
	FName TraceTag = "LineTraceTag";
	
	
};

