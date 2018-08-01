#pragma once

UENUM(BlueprintType)

enum class EMyCharMovement : uint8
{
	STAND UMETA(DisplayName = "Standing"),
	WALK UMETA(DisplayName = "Walking"),
	RUN UMETA(DisplayName = "Running"),
	CROUCH UMETA(DisplayName = "Crouching"),
	CLIMB UMETA(DisplayName = "Climbing"),
};
