// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	UMyCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void PhysClimbingLedge(float deltaTime, int32 Iterations);
	
	virtual void PhysClimbingLadder(float deltaTime, int32 Iterations);
	
};
