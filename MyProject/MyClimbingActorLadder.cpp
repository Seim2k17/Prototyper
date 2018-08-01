// Fill out your copyright notice in the Description page of Project Settings.

#include "MyClimbingActorLadder.h"




AMyClimbingActorLadder::AMyClimbingActorLadder()
{
	ClimbableTagName = "Ladder";
	ClimbingTrigger->ComponentTags.Add(ClimbableTagName);
}

void AMyClimbingActorLadder::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interact with the Ladder"));
	//@ToDo: Shoot Multicast (SetMovementMode)

	OnClimbingAndMovementChanged.ExecuteIfBound(EMyCharClimbingMode::ENTERLADDER, EMyCharMovement::CLIMB);
}
