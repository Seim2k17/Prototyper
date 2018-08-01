// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EMyCharClimbingMode.h"
#include "EMyCharMovement.h"
#include "CharacterBase.generated.h"


class UHealthComponent;

UCLASS()
class MYPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

/************************************************************************/
/* METHODS                                                              */
/************************************************************************/


public:

	ACharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Sets default values for this character's properties
	ACharacterBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMyCharMovement MovementMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMyCharClimbingMode ClimbingMode;

};
