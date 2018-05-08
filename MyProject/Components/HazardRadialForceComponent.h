// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "HazardRadialForceComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHazardRadialForceComponent : public URadialForceComponent
{
	GENERATED_BODY()
	
protected:

#if WITH_EDITOR
		void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	
};
