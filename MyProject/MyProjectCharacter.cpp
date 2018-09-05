// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/HealthComponent.h"
#include "MyProjectGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/MyCharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MyClimbTrackerComponent.h"
#include "MyProjectGameMode.h"
#include "MyClimbingActorBase.h"
#include "MyClimbingActorLadder.h"




//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	
	//why no see ?
	//HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	InitializeCharacter();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


AMyProjectCharacter::AMyProjectCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation

	ClimbingTrackerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ClimbingTracker"));
	ClimbingTrackerSphere->SetupAttachment(RootComponent);

	HeadTracker = CreateDefaultSubobject<USphereComponent>(TEXT("HeadTracker"));
	HeadTracker->SetupAttachment(RootComponent);
	HeadTracker->SetSphereRadius(10);
	HeadTracker->SetRelativeLocation(FVector(0, 0, -80));
	
	ClimbTracker = CreateDefaultSubobject<UMyClimbTrackerComponent>(TEXT("ClimbTrackerComponent"));

	InitializeCharacter();

	LadderClimbingMode = ELadderClimbingMode::none;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


void AMyProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MovementComp = Cast<UMyCharacterMovementComponent>(GetCharacterMovement());

}

void AMyProjectCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	fDeltaSeconds = DeltaTime;

	if (bDebugMessagesActive)
	{
		DrawDebugInfosOnScreen();
	}


	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, ZoomedInterpSpeed);

	FollowCamera->SetFieldOfView(NewFOV);

	

}


USphereComponent* AMyProjectCharacter::GetClimbingTrackerSphere()
{
	return ClimbingTrackerSphere;
}


//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyProjectCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProjectCharacter::OnResetVR);
	//PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyProjectCharacter::CheckInventory);
}


void AMyProjectCharacter::CheckInventory()
{
	if (InventoryHUD)
	{
		//when all Inventory is C++ then you can do this, but we need to get all Functionality in the EventGraph in the InventoryHUD.widget Class to C++ --> PITA !!!
// 		if (InventoryHUD->GetVisibility() == ESlateVisibility::Hidden)
// 		{
// 			bWantsToZoom = true;
// 			InventoryHUD->SetVisibility(ESlateVisibility::Visible);
// 		}
// 		else
// 		{
// 			bWantsToZoom = false;
// 			InventoryHUD->SetVisibility(ESlateVisibility::Hidden);
// 		}

		if (!bWantsToZoom)
		{
			bWantsToZoom = true;
		}
		else
		{
			bWantsToZoom = false;
		}

	}

}


void AMyProjectCharacter::InitializeCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ZoomedFOV = 65.f;
	ZoomedInterpSpeed = 20;

	ClimbingTrackerSphere->SetSphereRadius(150.0f);
	ClimbingTrackerSphere->SetRelativeLocation(FVector(100, 0, 0));


	ClimbLedgeTagName = "Ledge";
	ClimbLadderTagName = "Ladder";

}

void AMyProjectCharacter::BeginPlay()
{
	/** Don´t forget this EVER !*/
	Super::BeginPlay();


	DefaultFOV = FollowCamera->FieldOfView;

	/** ATM we use a BP Version !!!
	//Reference CharacterInventoryHUD
	AMyProjectGameMode* gm = Cast<AMyProjectGameMode>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			if (gm->InventoryWidgetClass)
			{
				InventoryHUD = CreateWidget<UUserWidget>(GetWorld(), gm->InventoryWidgetClass);
				if (InventoryHUD)
				{
					InventoryHUD->AddToViewport();
					InventoryHUD->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	*/
	ClimbingTrackerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ClimbingTrackerSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	ClimbingTrackerSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::ClimbingTrackerBeginOverlap);
	ClimbingTrackerSphere->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::ClimbingTrackerEndOverlap);

	HeadTracker->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::HeadTrackerBeginOverlap);
	HeadTracker->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::HeadTrackerEndOverlap);


	bDebugMessagesActive = true;


}


//ClimbingTrackerStuff
void AMyProjectCharacter::ClimbingTrackerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FName ClimbTagName;
	ClimbTagName = "NONAME";
	
	AMyClimbingActorBase* MyCA = Cast<AMyClimbingActorBase>(OtherActor);
	
	if (MyCA)
	{
		if (OtherComp->ComponentTags.Num() > 0)
		{
			if (OtherComp->ComponentTags[0] == ClimbLedgeTagName)
			{
				ClimbTagName = ClimbLedgeTagName;
				if (MovementMode != EMyCharMovement::CLIMB)
				{
					ClimbingMode = EMyCharClimbingMode::CANGRABLEDGE;
				}
				
			}
			if (OtherComp->ComponentTags[0] == ClimbLadderTagName)
			{
				ClimbTagName = ClimbLadderTagName;
				if (MovementMode != EMyCharMovement::CLIMB)
				{
					ClimbingMode = EMyCharClimbingMode::CANGRABLADDER;
				}
			}
			UE_LOG(LogTemp, Log, TEXT("Overlap in: %s, its a %s"), *OtherActor->GetName(), *ClimbTagName.ToString());
		}
	}


	AMyProjectGameMode* MyGm = Cast<AMyProjectGameMode>(GetWorld()->GetAuthGameMode());
	MyGm->ShowInteractionWidget();

	CurrentInteractibleReference = OtherActor;

	if (MyCA)
	{
		MyCA->OnClimbingAndMovementChanged.BindUFunction(this, FName("InteractCallback"));
	}

}

void AMyProjectCharacter::ClimbingTrackerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("ClimbingTrackerEndOverlap(): %s"), *OtherActor->GetName());
	//ClimbingMode = EMyCharClimbingMode::NONE;
	//ClimbingMode = EMyCharClimbingMode::EXITLADDERTOP;

	AMyProjectGameMode* MyGm = Cast<AMyProjectGameMode>(GetWorld()->GetAuthGameMode());
	MyGm->HideInteractionWidget();

	CurrentInteractibleReference = nullptr;
}


void AMyProjectCharacter::HeadTrackerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("HeadTrackerBeginOverlap(): "));
}


void AMyProjectCharacter::HeadTrackerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("HeadTrackerEndOverlap(): "));
}

void AMyProjectCharacter::InteractCallback(EMyCharClimbingMode ClimbingModeToChange, EMyCharMovement MovementModeToChange)
{
	switch (ClimbingModeToChange)
	{
	case EMyCharClimbingMode::NONE:
		break;
	case EMyCharClimbingMode::CANGRABLEDGE:
		break;
	case EMyCharClimbingMode::CANGRABLADDER:
		break;
	case EMyCharClimbingMode::CANGRABLEDGEANDLADDER:
		break;
	case EMyCharClimbingMode::ENTERLEDGE:
	{
		UE_LOG(LogTemp, Log, TEXT("InteractCallback(): I´m Climbing the ledge"));
		//@ToDo: Add CustomMovementModeImplementations and Changes in the ABP !
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(EMyCharClimbingMode::ENTERLEDGE));
		break;
	}

	case EMyCharClimbingMode::ENTERLADDER:
	{
		UE_LOG(LogTemp, Log, TEXT("InteractCallback(): I´m Climbing the ladder"));
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(EMyCharClimbingMode::ENTERLADDER));
		break;
	}

	case EMyCharClimbingMode::IDLELEDGE:
		break;
	case EMyCharClimbingMode::IDLELADDER:
		break;
	case EMyCharClimbingMode::EXITLADDERTOP:
	{
		UE_LOG(LogTemp, Log, TEXT("InteractCallback(): I´m Exiting the ladder"));
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(EMyCharClimbingMode::EXITLADDERTOP));
		break;
	}
		
	case EMyCharClimbingMode::EXITLADDERBTM:
	{
		UE_LOG(LogTemp, Log, TEXT("InteractCallback(): I´m Exiting the ladder"));
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(EMyCharClimbingMode::EXITLADDERBTM));
		break;
	}
		
	default:
		break;
	}

}

void AMyProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMyProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMyProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::MoveForward(float Value)
{
	switch (MovementMode)
	{
	case EMyCharMovement::CLIMB:
		if ((ClimbingMode != EMyCharClimbingMode::ENTERLADDER) &&
			(ClimbingMode != EMyCharClimbingMode::ENTERLEDGE) &&
			(ClimbingMode != EMyCharClimbingMode::EXITLADDERTOP) &&
			(ClimbingMode != EMyCharClimbingMode::EXITLADDERBTM))
		{


			UE_LOG(LogTemp, Log, TEXT("ClimbingLadder"))
			if (Value > 0)
			{
				ClimbingMode = EMyCharClimbingMode::CLIMBUPLADDER;
				//@ToDo: CheckClimb Back/Foward
				//UE_LOG(LogTemp, Log, TEXT("ClimbingUpLadder"));
			}
			if (Value == 0)
			{
				if ((ClimbingMode == EMyCharClimbingMode::CLIMBUPLADDER) || (ClimbingMode == EMyCharClimbingMode::CLIMBDOWNLADDER))
				{
					ClimbingMode = EMyCharClimbingMode::IDLELADDER;
				}
				//UE_LOG(LogTemp, Log, TEXT("IDLE-Ladder"));
			}
			if (Value < 0)
			{
				ClimbingMode = EMyCharClimbingMode::CLIMBDOWNLADDER;
				//UE_LOG(LogTemp, Log, TEXT("ClimbingDownLadder"));
			}
		}
		break;
		
		
	default:
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
		break;
	}
	
}

void AMyProjectCharacter::MoveRight(float Value)
{
	switch (MovementMode)
	{
	case EMyCharMovement::CLIMB:
		//@ToDo: CheckClimb Left/Right
		break;
	default:
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
		break;
	}

}

void AMyProjectCharacter::DrawDebugInfosOnScreen()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.000f, FColor::Green, FString::Printf(TEXT("Hm: %s"), true ? TEXT("true") : TEXT("false"), false));
	}

	FString MoveMode;
	FString ClimbMode;

	switch (MovementMode)
	{
	case EMyCharMovement::STAND:
		MoveMode = "IDLE";
		break;
	case EMyCharMovement::WALK:
		MoveMode = "Walking";
		break;
	case EMyCharMovement::CLIMB:
		MoveMode = "ClimbingMode";
		break;
	case EMyCharMovement::CROUCH:
		MoveMode = "Crouching";
		break;
	case EMyCharMovement::RUN:
		MoveMode = "Running";
		break;
	default:
		MoveMode = "NOT_SET";
		break;
	}

	switch (ClimbingMode)
	{
	case EMyCharClimbingMode::CANGRABLADDER:
		ClimbMode = "CanGrabLadder";
		break;
	case EMyCharClimbingMode::CANGRABLEDGE:
		ClimbMode = "CanGrabLedge";
		break;
	case EMyCharClimbingMode::CANGRABLEDGEANDLADDER:
		ClimbMode = "CanGrabLadderAndLedge";
		break;
	case EMyCharClimbingMode::ENTERLADDER:
		ClimbMode = "EnterLadder";
		break;
	case EMyCharClimbingMode::ENTERLEDGE:
		ClimbMode = "EnterLedge";
		break;
	case EMyCharClimbingMode::IDLELADDER:
		ClimbMode = "IdleLadder";
		break;
	case EMyCharClimbingMode::IDLELEDGE:
		ClimbMode = "IdleLedge";
		break;
	case EMyCharClimbingMode::CLIMBUPLADDER:
		ClimbMode = "ClimbUpLadder";
		break;
	case EMyCharClimbingMode::CLIMBDOWNLADDER:
		ClimbMode = "ClimbDownLadder";
		break;
	case EMyCharClimbingMode::NONE:
		ClimbMode = "INACTIVE";
		break;
	case EMyCharClimbingMode::EXITLADDERTOP:
		ClimbMode = "EXITONTOP";
		break;
	default:
		ClimbMode = "NOT_SET";
		break;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.000f, FColor::Red, FString::Printf(TEXT("MovementMode: %s"), *MoveMode), false);
		GEngine->AddOnScreenDebugMessage(-1, 0.000f, FColor::Red, FString::Printf(TEXT("ClimbingMode: %s"), *ClimbMode), false);
	}
	
}

void AMyProjectCharacter::Interact()
{
	if (CurrentInteractibleReference)
	{
		AMyClimbingActorBase* MyRef = Cast<AMyClimbingActorBase>(CurrentInteractibleReference);
		if (MyRef)
		{
			MyRef->Interact();
		}
	}
}

void AMyProjectCharacter::SetMovementMode(EMyCharClimbingMode ClimbingState, EMyCharMovement MovementState)
{
	ClimbingMode = ClimbingState;
	MovementMode = MovementState;

	//GetCharacterMovement()->SetMovementMode(MOVE_Custom, static_cast<uint8>(ClimbingMode));
}

void AMyProjectCharacter::SetLadderTopPosition()
{

	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
	if (CurrentClimbableActor)
	{
		AMyClimbingActorLadder* MyLadder = Cast<AMyClimbingActorLadder>(CurrentClimbableActor);
		FVector LadderExit = MyLadder->TargetPointExitTop->GetComponentLocation();
		LadderExit.Z = LadderExit.Z + MyLadder->DistanceHandsToTop;
		SetActorLocation(LadderExit);
	}
	

}

void AMyProjectCharacter::SetCurrentClimbingActor(AMyClimbingActorBase* ClimbActor)
{
	CurrentClimbableActor = ClimbActor;
}
