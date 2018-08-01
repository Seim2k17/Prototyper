// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyClimbingActorBase.h"
#include "MyClimbingActorLedge.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyClimbingActorLedge : public AMyClimbingActorBase
{
	GENERATED_BODY()
	
public:

	AMyClimbingActorLedge();

	virtual bool CanMoveRight() const override;

	virtual bool CanMoveLeft() const override;

	virtual void Interact() override;
	
};
