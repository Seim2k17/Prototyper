#pragma once

UENUM(BlueprintType)

enum class ELadderClimbingMode : uint8
{
	none UMETA(Displayname = "none"),
	TOPENTER UMETA(Displayname = "LadderEnterTop"),
	BTMENTER UMETA(Displayname = "LadderEnterBottom"),
	MIDDLEENTER UMETA(Displayname = "LadderEnterMiddle"),
};