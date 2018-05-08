// Fill out your copyright notice in the Description page of Project Settings.

#include "HazardRadialForceComponent.h"
#include "Components/PainCausingComponent.h"



#if WITH_EDITOR
void UHazardRadialForceComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	TArray<UPainCausingComponent*> PainComps;
	GetOwner()->GetComponents<UPainCausingComponent>(PainComps);
	//only use the first Component
	if (PainComps.Num() > 0) PainComps[0]->SetRadius(Radius);
}
#endif
