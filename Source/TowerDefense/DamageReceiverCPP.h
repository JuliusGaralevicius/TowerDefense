// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageReceiverCPP.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UDamageReceiverCPP : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOWERDEFENSE_API IDamageReceiverCPP
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		bool OnDamageReceived(float damage, FVector ImpactPoint);
};
