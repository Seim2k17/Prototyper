// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

//very own custom Event, available in Blueprint !
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginReadySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDiedSignature);

UCLASS( ClassGroup=(ImShaPrototype), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	//** ONLY FOR DEBUG OUTPUT REASON can be removed later*/
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float DefaultHealth;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBeginReadySignature OnBeginReady;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterDiedSignature OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Heal(float HealAmount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Character")
	float Health;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool bIsDead;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	
};
