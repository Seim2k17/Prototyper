#pragma once

UENUM(BlueprintType)

enum class EMyCharClimbingMode : uint8 
{
	NONE UMETA(DisplayName = "none"),
	CANGRABLEDGE UMETA(DisplayName = "CanGrabLedge"),
	CANGRABLADDER UMETA(DisplayName = "CanGrabLadder"),
	CANGRABLEDGEANDLADDER UMETA(DisplayName = "CanGrabLedegAndLadder"),
	ENTERLEDGE UMETA(DisplayName = "EnterLedge"),
	ENTERLADDER UMETA(DisplayName = "EnterLadder"),
	IDLELEDGE UMETA(DisplayName = "IdleLedge"),
	IDLELADDER UMETA(DisplayName = "IdleLadder"),
	CLIMBUPLADDER UMETA(DisplayName = "CLIMBUPLADDER"),
	CLIMBDOWNLADDER UMETA(DisplayName = "CLIMBDOWNLADDER"),
	EXITLADDERTOP UMETA(DisplayName = "EXITLADDERTOP"),
	EXITLADDERBTM UMETA(DisplayName = "EXITLADDERBTM"),


};
