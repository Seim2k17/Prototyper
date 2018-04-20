#pragma once
#pragma once

UENUM(BlueprintType)

enum class ECustomTriggerShapes : uint8
{
	none UMETA(Displayname = "none"),
	BoxTrigger UMETA(Displayname = "BoxTrigger"),
	SphereTrigger UMETA(Displayname = "SphereTrigger"),
	CapsuleTrigger UMETA(Displayname = "CapsuleTrigger"),
};