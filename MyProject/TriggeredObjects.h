// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredObjects.generated.h"

UCLASS()
class MYPROJECT_API ATriggeredObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeredObjects();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hints")
		FName TriggerWhat;

	UFUNCTION(BlueprintCallable, Category = "TriggeredObjects")
		virtual void TriggerSomething();

	
};
