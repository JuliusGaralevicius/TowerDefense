// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "DestructibleMesh.h"

#include "StaticLibrary.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDestructiblePair
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDestructibleMesh * destructible = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh * staticMesh = NULL;
};

UCLASS()
class TOWERDEFENSE_API UStaticLibrary : public UObject
{
	GENERATED_BODY()
	
};
