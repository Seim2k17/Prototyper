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

	bDoThingsOnce = false;

	UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): Movement Mode changed ..."));

	AMyProjectCharacter* MyChar = Cast<AMyProjectCharacter>(CharacterOwner);

	switch (CustomMovementMode)
	{
		case EMyCharClimbingMode::ENTERLADDER:
		{
			bDoThingsOnce = true;
			UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... ENTERLADDER."));
			MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyChar->ClimbingTrackerSphere->SetEnableGravity(false);
			MyChar->GetMesh()->SetEnableGravity(false);
			MyChar->GetCapsuleComponent()->SetEnableGravity(false);
			MyChar->MovementMode = EMyCharMovement::CLIMB;
			MyChar->ClimbingMode = EMyCharClimbingMode::ENTERLADDER;
			
			break;
		}
		case EMyCharClimbingMode::ENTERLEDGE:
		{
			bDoThingsOnce = true;
			MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyChar->ClimbingTrackerSphere->SetEnableGravity(false);
			MyChar->GetMesh()->SetEnableGravity(false);
			MyChar->GetCapsuleComponent()->SetEnableGravity(false);
			MyChar->MovementMode = EMyCharMovement::CLIMB;
			MyChar->ClimbingMode = EMyCharClimbingMode::ENTERLEDGE;
			UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... ENTERLEDGE."));
			break;
		}
		case EMyCharClimbingMode::EXITLADDERTOP:
		{
			bDoThingsOnce = true;
			MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MyChar->ClimbingTrackerSphere->SetEnableGravity(true);
			MyChar->GetMesh()->SetEnableGravity(true);
			MyChar->GetCapsuleComponent()->SetEnableGravity(true);
			//MyChar->MovementMode = EMyCharMovement::STAND;
			//MyChar->SetLadderTopPosition();
			MyChar->ClimbingMode = EMyCharClimbingMode::EXITLADDERTOP;
			//MyChar->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... EXITTOPLADDER."));
			break;
		}
		case EMyCharClimbingMode::EXITLADDERBTM:
		{
			bDoThingsOnce = true;
			MyChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MyChar->ClimbingTrackerSphere->SetEnableGravity(true);
			MyChar->GetMesh()->SetEnableGravity(true);
			MyChar->GetCapsuleComponent()->SetEnableGravity(true);
			MyChar->ClimbingMode = EMyCharClimbingMode::EXITLADDERBTM;
			UE_LOG(LogTemp, Log, TEXT("OnMovementModeChanged(): ... EXITBTMLADDER."));
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
		if (bDoThingsOnce)
		{
			UE_LOG(LogTemp, Log, TEXT("PhysCustom(): ENTERLADDER."));
			bDoThingsOnce = false;
		}
		
		//@ToDo: ....
		PhysClimbingLadder(deltaTime, Iterations);
		break;
	}
	case EMyCharClimbingMode::ENTERLEDGE:
	{
		if (bDoThingsOnce)
		{
			UE_LOG(LogTemp, Log, TEXT("PhysCustom(): ENTERLEDGE."));
			bDoThingsOnce = false;
		}
		PhysClimbingLedge(deltaTime, Iterations);;
		break;
	}

	case EMyCharClimbingMode::EXITLADDERTOP:
	{
		if (bDoThingsOnce)
		{
			UE_LOG(LogTemp, Log, TEXT("PhysCustom(): EXITLADDERTOP."));
			bDoThingsOnce = false;
		}
		PhysClimbingLedge(deltaTime, Iterations);;
		break;
	}
	case EMyCharClimbingMode::EXITLADDERBTM:
	{
		if (bDoThingsOnce)
		{
			UE_LOG(LogTemp, Log, TEXT("PhysCustom(): EXITLADDERBTM."));
			bDoThingsOnce = false;
		}
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

	RestorePreAdditiveRootMotionVelocity();


	ApplyRootMotionToVelocity(deltaTime);
	
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
	//UE_LOG(LogTemp, Log, TEXT("JetzAbor"));

	/** A SELF TRY ^^*/
	AMyProjectCharacter* MyChar = Cast<AMyProjectCharacter>(GetOwner());
	if (MyChar)
	{
		FVector MyCharPos = GetActorLocation();
		
		if (MyChar->ClimbingMode == EMyCharClimbingMode::CLIMBUPLADDER)
		{
			UE_LOG(LogTemp, Log, TEXT("PhysClimbingLadder: JetzAborUp"));
			MyCharPos.Z = MyCharPos.Z + 2.5f;
			//MyChar->AddMovementInput(MyCharPos);
			MyChar->SetActorLocation(MyCharPos);
		}

		if (MyChar->ClimbingMode == EMyCharClimbingMode::CLIMBDOWNLADDER)
		{
			UE_LOG(LogTemp, Log, TEXT("PhysClimbingLadder: JetzAborDown"));
			MyCharPos.Z = MyCharPos.Z - 2.5f;
			//MyChar->AddMovementInput(MyCharPos);
			MyChar->SetActorLocation(MyCharPos);
		}
		
		
// 		if (MyChar->ClimbingMode == EMyCharClimbingMode::EXITLADDERTOP)
// 		{
// 			MyChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
// 		}
		
		

	}
	

	//From Hoat:
	
	/**
	RestorePreAdditiveRootMotionVelocity();

	//const bool bCalculateVeloctiy =
		!bRootMotionDrivenLadderClimbingOnly && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity();

	// CalcVelocity(deltaTime, false, false, false);
	Velocity = FVector::ZeroVector;
	AHoatPlayerCharacter* hoatCharacter = Cast<AHoatPlayerCharacter>(CharacterOwner);
	switch (OnLadderMovementState)
	{
	case EOnLadderMovementState::OnLadder_EnterFromAir:
	case EOnLadderMovementState::OnLadder_GrabLadder:
	{
		hoatCharacter->SetActorLocation(GrabTargetLocation);

		const FRotator actorRotation = hoatCharacter->GetActorRotation();
		hoatCharacter->SetActorRotation(FRotator(actorRotation.Pitch, GrabTargetYaw, actorRotation.Roll));

		OnLadderMovementState = EOnLadderMovementState::OnLadder_NotMoving;
	}
	break;
	case EOnLadderMovementState::OnLadder_EnterTopTurnLeft:
	case EOnLadderMovementState::OnLadder_EnterTopTurnRight:
	{
		if (!DoAutoSaveGrab(deltaTime))
		{
			OnLadderMovementState = EOnLadderMovementState::OnLadder_NotMoving;
		}
	}
	break;
	default:
	{
		OnLadderMovementState = EOnLadderMovementState::OnLadder_NotMoving;

		if (bPendingLedgeJump || !bFinishedLedgeJump)
		{
			break;
		}

		DesiredLedgeJumpType = EHoatLedgeJumpType::None;
		const bool bCanMoveUp = CurrentClimbableActor->CanMoveUp(
			hoatCharacter->GetActorLocation(), hoatCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		const bool bCanMoveDown = CurrentClimbableActor->CanMoveDown(hoatCharacter->GetActorLocation(), 1.0f);

		bool bUnfinishedJumps = PendingLadderJumps.Num() > 0;
		bool bPendingJumps = bUnfinishedJumps && PendingLadderJumps[PendingLadderJumps.Num() - 1];
		if (bUnfinishedJumps && !bCanMoveUp)
		{
			bPullUpLedge = true;
			break;
		}
		else if (bPendingJumps && bUnfinishedJumps)
		{
			OnLadderMovementState = EOnLadderMovementState::OnLadder_JumpUp;
		}
		else if (!Acceleration.IsZero())
		{
			const FVector AccelDir = Acceleration.GetSafeNormal();
			const FVector Up = UpdatedComponent->GetUpVector();

			const float angleToUp = GetAngle(Up, AccelDir);

			const bool bMoveUp = IsAllowedInputAngle(angleToUp);

			const FVector Down = -Up;
			const float angleToDown = GetAngle(Down, AccelDir);

			const bool bMoveDown = IsAllowedInputAngle(angleToDown);

			if (bMoveUp)
			{
				if (bCanMoveUp)
				{
					OnLadderMovementState = EOnLadderMovementState::OnLadder_MovingUp;
					Velocity = (bCalculateVeloctiy) ? Up * LadderClimbSpeed : FVector::ZeroVector;
				}
				else
				{
					bPullUpLedge = true;
					break;
				}
			}
			else if (bMoveDown)
			{
				if (bCanMoveDown)
				{
					OnLadderMovementState = EOnLadderMovementState::OnLadder_SlideDown;
					Velocity = (bCalculateVeloctiy) ? Up * LadderSlideDownSpeed : FVector::ZeroVector;
				}
				else
				{
					LetGo();
					return;
				}
			}
			else
			{
				const FVector Right = UpdatedComponent->GetRightVector();

				const float angleToRight = GetSignedAngle(Right, AccelDir);
				bool bJumpRight = IsAllowedInputAngle(MAX_ANGLE - AllowedInputAngle, angleToRight);

				const FVector Left = -Right;
				const float angleToLeft = GetSignedAngle(Left, AccelDir);
				bool bJumpLeft = IsAllowedInputAngle(MAX_ANGLE - AllowedInputAngle, angleToLeft);

				AClimbableActor* targetActor;
				if (bJumpLeft && CanDoLedgeJumpInDirection(Left, targetActor))
				{
					DesiredLedgeJumpType = EHoatLedgeJumpType::LadderToLedge_Left;
				}
				else if (bJumpRight && CanDoLedgeJumpInDirection(Right, targetActor))
				{
					DesiredLedgeJumpType = EHoatLedgeJumpType::LadderToLedge_Right;
				}
			}
		}
	}
	break;
	}

	ApplyRootMotionToVelocity(deltaTime);

	Iteration++;
	bJustTeleported = false;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);

	bool bCheckForCollisions = !bPendingLedgeJump && bFinishedLedgeJump;
	if (!SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), bCheckForCollisions, Hit) &&
		OnLadderMovementState == EOnLadderMovementState::OnLadder_SlideDown)
	{
		LetGo();
		return;
	}

	ApplyLedgeJumpZCoordinateCorrection();

	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}

	**/
}
