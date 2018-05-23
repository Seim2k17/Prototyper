// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMyProjectGameMode::StartPlay()
{
	GameState->HandleBeginPlay();

	if (IAWidgetClass)
	{
		InteractionHUD = CreateWidget<UUserWidget>(GetWorld(), IAWidgetClass);
		if (InteractionHUD)
		{
			InteractionHUD->AddToViewport();
			InteractionHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMyProjectGameMode::ShowInteractionWidget()
{
	if (IAWidgetClass && InteractionHUD)
	{
		InteractionHUD->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		UE_LOG(LogTemp, Log, TEXT("No Interaction HUD Class found or Connection broken. Please check Game-Mode"));
	}
}

void AMyProjectGameMode::HideInteractionWidget()
{
	if (IAWidgetClass && InteractionHUD)
	{
		InteractionHUD->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No Interaction HUD Class found or Connection broken. Please check Game-Mode"));
	}
}
