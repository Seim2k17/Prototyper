// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupActorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "MyProjectGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectCharacter.h"


// Sets default values
APickupActorBase::APickupActorBase()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;

	PickupTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTrigger"));

	PickupTrigger->SetupAttachment(PickupMesh);

	/**Get ItemDatatable-Data (Move To  GameMode ?)*/
	//ConstructorHelpers::FObjectFinder<UDataTable>ItemDatatable_BP(TEXT("DataTable'/Game/Ui/Inventory/DT_PickupTable.DT_PickupTable'"));
	ConstructorHelpers::FObjectFinder<UDataTable>ItemDatatable_BP(*ItemDataTablePath);
	ItemDataTableReference = ItemDatatable_BP.Object;

	bInteractionAllowed = false;

}

// Called when the game starts or when spawned
void APickupActorBase::BeginPlay()
{
	Super::BeginPlay();
	

}

void APickupActorBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//make sure not to loose BP functionality
	Super::NotifyActorBeginOverlap(OtherActor);

	AMyProjectGameMode* gm = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gm->ShowInteractionWidget();

	bInteractionAllowed = true;
	gm->OnOverlapItem.Broadcast(Itemname);

	//**TMP until better solution is found*
	AMyProjectCharacter* MyChar = Cast<AMyProjectCharacter>(OtherActor);
	if (MyChar)
	{
		MyChar->CurrentInteractibleReference = this;
	}


	//Get Data from Datatable (Name,Text:A, Text:B)

	//da InventoryStruct geblueprinted ist und kein CPP Struct -> Set Params via BlueprintFunction im ConstructScript ! --> 
	//wenn aber später inventory struct gec++ t wird hier Datatable suche ausbauen
// 	static const FString ContextString(TEXT("i01_Potion_Heal50"));
// 	ItemDataTableReference->FindRow()

}

void APickupActorBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	//make sure not to loose BP functionality
	Super::NotifyActorEndOverlap(OtherActor);

	AMyProjectGameMode* gm = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gm->HideInteractionWidget();

	bInteractionAllowed = false;

	//**TMP until better solution is found*
	AMyProjectCharacter* MyChar = Cast<AMyProjectCharacter>(OtherActor);
	if (MyChar)
	{
		MyChar->CurrentInteractibleReference = nullptr;
	}
	
}

void APickupActorBase::ChangeItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("PATH"));
	UStaticMesh* Asset = MeshAsset.Object;
	PickupMesh->SetStaticMesh(Asset);
}

/** If Itemchanged  assign Mesh,Material and Properties dynamically*/
/** Atm this is done in Blueprint via the ConstructionScript*/
void APickupActorBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	

	//Get Data from Datatable (Name,Text:A, Text:B)
	//later in extra function ChangeItem();
}

