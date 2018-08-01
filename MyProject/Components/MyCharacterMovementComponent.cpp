// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacterMovementComponent.h"
#include "EMyCharClimbingMode.h"
#include "EMyCharMovement.h"
#include "MyProjectCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"




UMyCharacterMovementComponent::UMyCharacterMovementComponent(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UMyCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UMyCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	UE_LOG(LogTemp, Log, TEXT("Movement Mode changed ..."));

	AMyProjectCharacter* MyChar = Cast<AMyProjectCharacter>(CharacterOwner);

	switch (CustomMovementMode)
	{
		case EMyCharClimbingMode::ENTERLADDER:
		{
			UE_LOG(LogTemp, Log, TEXT("... ENTERLADDER."));
			MyChar->MovementMode = EMyCharMovement::CLIMB;
			MyChar->ClimbingMode = EMyCharClimbingMode::ENTERLADDER;
			break;
		}
		case EMyCharClimbingMode::ENTERLEDGE:
		{
			MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyChar->ClimbingTrackerSphere->SetEnableGravity(false);
			MyChar->GetMesh()->SetEnableGravity(false);
			MyChar->GetCapsuleComponent()->SetEnableGravity(false);
			MyChar->MovementMode = EMyCharMovement::CLIMB;
			MyChar->ClimbingMode = EMyCharClimbingMode::ENTERLEDGE;
			UE_LOG(LogTemp, Log, TEXT("... ENTERLEDGE."));
			break;
		}
	}
}

void UMyCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	switch (CustomMovementMode)
	{
	case EMyCharClimbingMode::ENTERLADDER:
	{
		UE_LOG(LogTemp, Log, TEXT("ENTERLADDER."));
		//@ToDo: ....
		PhysClimbingLedge(deltaTime, Iterations);
		break;
	}
	case EMyCharClimbingMode::ENTERLEDGE:
	{
		UE_LOG(LogTemp, Log, TEXT("ENTERLEDGE."))
		PhysClimbingLedge(deltaTime, Iterations);;
		break;
	}
	default:
		break;
	}
}

void UMyCharacterMovementComponent::PhysClimbingLedge(float deltaTime, int32 Iterations)
{
	//TODo: updates the movement take modified walking state ! ATM FlyingState active

	/** Following is copypasted from CharacterMovementComponent::PhysFlying and ajdusted to Phyclimbing*/

	//RestorePreAdditiveRootMotionVelocity();


	//ApplyRootMotionToVelocity(deltaTime);

	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.Time < 1.f)
	{
		const FVector GravDir = FVector(0.f, 0.f, -1.f);
		const FVector VelDir = Velocity.GetSafeNormal();
		const float UpDown = GravDir | VelDir;

		bool bSteppedUp = false;
		if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
		{
			float stepZ = UpdatedComponent->GetComponentLocation().Z;
			bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
			if (bSteppedUp)
			{
				OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);
			}
		}

		if (!bSteppedUp)
		{
			//adjust and try again
			HandleImpact(Hit, deltaTime, Adjusted);
			SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		}
	}

	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}

}

void UMyCharacterMovementComponent::PhysClimbingLadder(float deltaTime, int32 Iterations)
{

}
