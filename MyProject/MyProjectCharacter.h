// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "ELadderClimbingMode.h"
#include "MyProjectCharacter.generated.h"

class UHealthComponent;
class UMyCharacterMovementComponent;
class UMyClimbTrackerComponent;
class USphereComponent;

UCLASS(config=Game)
class AMyProjectCharacter : public ACharacterBase
{
	GENERATED_BODY()

/************************************************************************/
/* METHODS                                                              */
/************************************************************************/

public:
	AMyProjectCharacter();

	AMyProjectCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Events")
	// 	void Interact();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FName ClimbLedgeTagName;

	FName ClimbLadderTagName;

	USphereComponent* GetClimbingTrackerSphere();

	void DrawDebugInfosOnScreen();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementMode(EMyCharClimbingMode ClimbingState, EMyCharMovement MovementState);

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void SetLadderTopPosition();

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void SetCurrentClimbingActor(AMyClimbingActorBase* ClimbActor);

		

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = "Zoom")
	void CheckInventory();

	//DONT FORGET THIS EVER (UFUNCTION) !!!!
	UFUNCTION()
	void ClimbingTrackerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//DONT FORGET THIS EVER (UFUNCTION) !!!!
	UFUNCTION()
	void ClimbingTrackerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void HeadTrackerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void HeadTrackerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void InteractCallback(EMyCharClimbingMode ClimbingModeToChange, EMyCharMovement MovementModeToChange);

private:

	/** METHODS */
	void InitializeCharacter();

/************************************************************************/
/* MEMBER                                                               */
/************************************************************************/

public:

	UMyCharacterMovementComponent* MovementComp;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//**Current Interactible available to the Player*
	UPROPERTY(BlueprintReadWrite, Category = "Interactibles")
	AActor* CurrentInteractibleReference;
	//why no see ?
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
// 	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadWrite, Category = "Equipment")
	UUserWidget* InventoryHUD;


	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMyClimbTrackerComponent* ClimbTracker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugMessagesActive;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* ClimbingTrackerSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* HeadTracker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Climbing")
	ELadderClimbingMode LadderClimbingMode;

	
	
protected:


	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1f, ClampMax = 100.f))
	float ZoomedInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	/** Default Field of View set during BeginPlay */
	float DefaultFOV;

	bool bWantsToZoom;

	float fDeltaSeconds;

private:

	AMyClimbingActorBase* CurrentClimbableActor;

};

