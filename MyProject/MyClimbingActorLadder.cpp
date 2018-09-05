// Fill out your copyright notice in the Description page of Project Settings.

#include "MyClimbingActorLadder.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Kismet//GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "MyProjectCharacter.h"
#include "ELadderClimbingMode.h"





AMyClimbingActorLadder::AMyClimbingActorLadder()
{
	PrimaryActorTick.bCanEverTick = true;
	ClimbableTagName = "Ladder";
	ClimbingTrigger->ComponentTags.Add(ClimbableTagName);

	DestinationTraceTreshhold = 105.0f;

	TargetPointExitTop = CreateDefaultSubobject<UChildActorComponent>(TEXT("ExitTopPoint"));
	TargetPointExitTop->SetupAttachment(SceneRoot);
	TargetPointExitTop->SetRelativeLocation(FVector(0, 0, 200));

	EnterTopPosition = CreateDefaultSubobject<UChildActorComponent>(TEXT("EnterTopPosition"));
	EnterTopPosition->SetupAttachment(SceneRoot);
	//just value for dummy 2x ladder meshheight ^^
	EnterTopPosition->SetRelativeLocation(FVector(-45.0f, 0, 405.0f));

	DistanceHandsToTop = 100.0f;
	DistanceBottomExit = 95.0f;

	bCharacterNearExitPosition = false;

}

void AMyClimbingActorLadder::BeginPlay()
{
	Super::BeginPlay();

	bCharacterWalksToEnterDestination = false;
	bCharacterNearExitPosition = false;
	//just a big number for init
	DestinationToUpTrace = 50000.0f;

	ClimbingTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMyClimbingActorLadder::OnHandleBeginOverlap);
	ClimbingTrigger->OnComponentEndOverlap.AddDynamic(this, &AMyClimbingActorLadder::OnHandleEndOverlap);
}


void AMyClimbingActorLadder::Tick(float DeltaTime)
{
	if (bCharacterWalksToEnterDestination)
	{

		CheckEnterPositionReached();
	}

	if (bCharacterNearExitPosition)
	{
		CheckExitPositionReached();
	}
	

}

void AMyClimbingActorLadder::OnHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void AMyClimbingActorLadder::OnHandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//get out on Top with head
	AMyProjectCharacter* MyChar = Cast<AMyProjectCharacter>(OtherActor);
	if ((MyChar->ClimbingMode == EMyCharClimbingMode::CLIMBUPLADDER) && 
		MyChar && 
		MyChar->MovementMode == EMyCharMovement::CLIMB)
	{
		
	}
	
}

void AMyClimbingActorLadder::Interact()
{
	ACharacter* MyChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMyProjectCharacter* MyProjectChar = Cast<AMyProjectCharacter>(MyChar);

	UE_LOG(LogTemp, Log, TEXT("Interact with the Ladder"));
	//@ToDo: Shoot Multicast (SetMovementMode)

	//only SinglePlayer !!!
	FVector StartP = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector LadderBtm = EnterPosition->GetComponentLocation();
	FVector LadderTop = TargetPointExitTop->GetComponentLocation();

	if (EnterPosition && TargetPointExitTop)
	{
		//initialize distanceValues
		FVector CurrentTracePos = MyChar->GetActorLocation();
		DestinationToUpTrace = (CurrentTracePos - EnterTopPosition->GetComponentLocation()).Size();
		DestinationToDownTrace = (EnterPosition->GetComponentLocation() - CurrentTracePos).Size();


		bCharacterNearExitPosition = true;

		if (FVector::Dist(StartP, LadderBtm) < FVector::Dist(StartP, LadderTop))
		{
			UE_LOG(LogTemp, Log, TEXT("Enter Ladder Bottom"));
			CurrentLadderEnterPosition = LadderBtm;
			MyProjectChar->LadderClimbingMode = ELadderClimbingMode::BTMENTER;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Enter Ladder Top"));
			CurrentLadderEnterPosition = LadderTop;
			MyProjectChar->LadderClimbingMode = ELadderClimbingMode::TOPENTER;
		}


		UNavigationSystem::SimpleMoveToLocation(GetWorld()->GetFirstPlayerController(), CurrentLadderEnterPosition);
		bCharacterWalksToEnterDestination = true;

		QueryHitParams.AddIgnoredActor(this);
		QueryHitParams.bTraceComplex = true;
		QueryHitParams.TraceTag = TraceTag;
		GetWorld()->DebugDrawTraceTag = TraceTag;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Please check Enter and LadderExitTargetPoints."));
	}
}


void AMyClimbingActorLadder::CheckEnterPositionReached()
{
	
	UE_LOG(LogTemp, Log, TEXT("CheckEnterPositionReached(): %s"), *FString::SanitizeFloat(DestinationToUpTrace));

	if (DestinationToUpTrace > DestinationTraceTreshhold)
	{
		//SingleLinetrace to Enterpoint for calculating the destination to the EnterPoint
		FVector StartTracePos =  UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation();
		GetWorld()->LineTraceSingleByChannel(TraceHit, StartTracePos, CurrentLadderEnterPosition, ECollisionChannel::ECC_Visibility, QueryHitParams);
		DestinationToUpTrace = (CurrentLadderEnterPosition - StartTracePos).Size();
		//GetWorld()->LineTraceSingleByChannel(TraceHit, StartTracePos, EnterPosition->GetComponentLocation(), ECollisionChannel::ECC_Visibility, QueryHitParams);
		//DestinationTrace = (EnterPosition->GetComponentLocation() - StartTracePos).Size() ;
	}
	else
	{
		ACharacter* MyChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AMyProjectCharacter* MyProjectChar = Cast<AMyProjectCharacter>(MyChar);
		//FVector LadderEnter = EnterPosition->GetComponentLocation();
		//LadderEnter.Z = LadderEnter.Z + MyChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		if (MyProjectChar->LadderClimbingMode == ELadderClimbingMode::BTMENTER)
		{
			CurrentLadderEnterPosition.Z += MyChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		}
		if (MyProjectChar->LadderClimbingMode == ELadderClimbingMode::TOPENTER)
		{
			CurrentLadderEnterPosition = EnterTopPosition->GetComponentLocation();
			CurrentLadderEnterPosition.Z -= MyChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		}

		bCharacterWalksToEnterDestination = false;
		//DestinationToUpTrace = 0;
		MyChar->SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
		//MyChar->SetActorLocation(LadderEnter);
		MyChar->SetActorLocation(CurrentLadderEnterPosition);
		if (MyProjectChar)
		{
			MyProjectChar->SetCurrentClimbingActor(this);
		}
		OnClimbingAndMovementChanged.ExecuteIfBound(EMyCharClimbingMode::ENTERLADDER, EMyCharMovement::CLIMB);

	}
}

//NEEDS REFACTOR. just 4PrototypeUSE
void AMyClimbingActorLadder::CheckExitPositionReached()
{

	
	UE_LOG(LogTemp, Log, TEXT("CheckExitTopPositionReached(): %s"), *FString::SanitizeFloat(DestinationToUpTrace));
	UE_LOG(LogTemp, Log, TEXT("CheckExitBottomPositionReached(): %s"), *FString::SanitizeFloat(DestinationToDownTrace));

	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMyProjectCharacter* MyChar = Cast<AMyProjectCharacter>(Char);

	if (MyChar && MyChar->ClimbingMode == EMyCharClimbingMode::CLIMBUPLADDER )
	{
		if (DestinationToUpTrace > DistanceHandsToTop)
		{
			//SingleLinetrace to Enterpoint for calculating the destination to the EnterPoint
			FVector CurrentTracePos = Char->GetActorLocation();
			//WHY LINETRACE DUDE?
			//GetWorld()->LineTraceSingleByChannel(TraceHit, CurrentTracePos, EnterPosition->GetComponentLocation(), ECollisionChannel::ECC_Visibility, QueryHitParams);

			//DestinationToUpTrace = (TargetPointExitTop->GetComponentLocation() - CurrentTracePos).Size();
			DestinationToUpTrace = (CurrentTracePos - TargetPointExitTop->GetComponentLocation()).Size();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("OnHandleEndOverlap(CALadder): MoveOut of Ladder-Trigger."));
			bCharacterNearExitPosition = false;
			DestinationToUpTrace = 0;
			//MyChar->SetLadderTopPosition();
			OnClimbingAndMovementChanged.ExecuteIfBound(EMyCharClimbingMode::EXITLADDERTOP, EMyCharMovement::CLIMB);
		}
	}

	if (MyChar && MyChar->ClimbingMode == EMyCharClimbingMode::CLIMBDOWNLADDER)
	{

		if (DestinationToDownTrace > DistanceBottomExit)
		{
			//SingleLinetrace to Enterpoint for calculating the destination to the EnterPoint
			FVector CurrentTracePos = Char->GetActorLocation();
			DestinationToDownTrace = (EnterPosition->GetComponentLocation() - CurrentTracePos).Size();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("OnHandleEndOverlap(CALadder): MoveOut of Ladder-Trigger."));
			bCharacterNearExitPosition = false;
			DestinationToUpTrace = 0;
			//MyChar->SetLadderTopPosition();
			OnClimbingAndMovementChanged.ExecuteIfBound(EMyCharClimbingMode::EXITLADDERBTM, EMyCharMovement::CLIMB);
		}

	}
	


	
}

