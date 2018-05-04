// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredObjectNames.h"
#include "TriggeredObjectBase.generated.h"

UCLASS()
class MYPROJECT_API ATriggeredObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeredObjectBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TriggeredObjects")
		void TriggerSomethingOverlapIn(ETriggeredObjectNames ObjectName);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "TriggeredObjects")
		void TriggerSomethingOverlapOut(ETriggeredObjectNames ObjectName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

/** MEMEBERS*/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggeredObjects")
		ETriggeredObjectNames TriggeredObjectName;
	
};
