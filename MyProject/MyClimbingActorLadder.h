// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyClimbingActorBase.h"
#include "MyClimbingActorLadder.generated.h"

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
	
	
};
