// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EItems.h"
#include "PickupActorBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UDataTable;
class UWidget;

UCLASS()
class MYPROJECT_API APickupActorBase : public AActor
{
	GENERATED_BODY()
		
public:	

	/**METHODS*/

	// Sets default values for this actor's properties
	APickupActorBase();

	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void NotifyActorEndOverlap(AActor* OtherActor) override;

	/**MEMBER*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* PickupTrigger;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	EItems Itemname;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	bool bInteractionAllowed;


protected:

	/**METHODS*/

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ChangeItem();

	/**MEMBER*/


	//UPROPERTY(EditInstanceOnly, Category = "Item")
	UDataTable* ItemDataTableReference;

	
 	


private:

	const FString ItemDataTablePath = "DataTable'/Game/Ui/Inventory/DT_PickupTable.DT_PickupTable'";
	
	/**METHODS*/

	/**MEMBER*/

	
	
};
