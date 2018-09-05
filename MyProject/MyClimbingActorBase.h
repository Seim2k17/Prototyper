// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EMyCharClimbingMode.h"
#include "EMyCharMovement.h"
#include "MyClimbingActorBase.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UChildActorComponent;
class UArrowComponent;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnClimbingAndMovementChangedSignature, EMyCharClimbingMode, ClimbingModeToChange, EMyCharMovement, MovementModeToChange);

UCLASS()
class MYPROJECT_API AMyClimbingActorBase : public AActor
{
	GENERATED_BODY()


//** METHODS **//

public:	
	// Sets default values for this actor's properties
	AMyClimbingActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//** MEMBERS **//

public:

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ClimbingMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* ClimbingTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Specs")
	FName ClimbableTagName;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* EnterPosition;

	virtual bool CanMoveUp() const;

	virtual bool CanMoveDown() const;

	virtual bool CanMoveRight() const;

	virtual bool CanMoveLeft() const;
	
	virtual void Interact();

	UPROPERTY()
	FOnClimbingAndMovementChangedSignature OnClimbingAndMovementChanged;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* DirectionArrow;
	
};
