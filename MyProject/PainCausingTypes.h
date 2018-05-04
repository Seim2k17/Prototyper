#pragma once

UENUM(BlueprintType)

enum class EPainCausingTypes : uint8
{
	JustPain UMETA(DisplayName = "Just Pain"),
	PointDamage UMETA(DisplayName = "PointDamage"),
	RadialDamage UMETA(DisplayName = "RadialDamage"),
	Pain4 UMETA(DisplayName = "futureStuff"),
};