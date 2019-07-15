// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarCPP.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UHealthBarCPP : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Health Change")
	void OnHealthChange(float maximumHealth, float currentHealth);
};
