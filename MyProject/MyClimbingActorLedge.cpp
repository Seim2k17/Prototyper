// Fill out your copyright notice in the Description page of Project Settings.

#include "MyClimbingActorLedge.h"






AMyClimbingActorLedge::AMyClimbingActorLedge()
{
	ClimbableTagName = "Ledge";
	ClimbingTrigger->ComponentTags.Add(ClimbableTagName);
}

bool AMyClimbingActorLedge::CanMoveRight() const
{
	return true;
}

bool AMyClimbingActorLedge::CanMoveLeft() const
{
	return true;
}

void AMyClimbingActorLedge::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interact with the Ledge"));
	OnClimbingAndMovementChanged.ExecuteIfBound(EMyCharClimbingMode::ENTERLEDGE, EMyCharMovement::CLIMB);
}
