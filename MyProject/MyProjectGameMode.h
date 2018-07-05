// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameMode.generated.h"

class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlapPickupSignature, EItems, ItemName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlapHotspotSignature, EInteractibles, HotspotName);


UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();


	UFUNCTION(BlueprintCallable, Category = Game)
	virtual void StartPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void ShowInteractionWidget();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void HideInteractionWidget();

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<class UUserWidget> IAWidgetClass;
	
	UUserWidget* InteractionHUD;

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<class UUserWidget> InventoryWidgetClass;

	//InventoryRef in MyProjectCharacter !

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnOverlapPickupSignature OnOverlapItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnOverlapHotspotSignature OnOverlapHotspot;

};



