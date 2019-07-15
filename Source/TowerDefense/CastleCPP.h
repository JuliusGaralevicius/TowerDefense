// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CastleWall.h"
#include "HealthBarCPP.h"
#include "CastleCPP.generated.h"

struct FAttackLocation
{
	FVector location = FVector::ZeroVector;
	bool available = true;
};

UCLASS()
class TOWERDEFENSE_API ACastleCPP : public AActor
{
	GENERATED_BODY()
	
public:	

	ACastleCPP();
	TArray<UChildActorComponent*> Walls;
	TArray<FAttackLocation*> AttackLocations;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CreateWallComponent(int index);

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform & Transform) override;
	UFUNCTION()
	void WallExploded(int WallID, float HealthPercentage);


	UPROPERTY(BlueprintAssignable)
	FOnWallExplode onWallExplode;

	UPROPERTY(EditAnywhere)
	int AttackLocationCount = 30;

	/* How many goblins can a tree consume until game over */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 totalGoblinCapacity = 10;

	/* How many goblins have been consumed so far */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 goblinsConsumed = 0;

	UPROPERTY(EditAnywhere)
	int AttackLocationDistance = 3000.0f;

	UPROPERTY(EditAnywhere)
	UClass* DebugActorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHealthBarCPP> healthBarClass;

	UPROPERTY(EditAnywhere)
	float healthBarHeightOffset = 2000.0f;

	UFUNCTION(BlueprintCallable)
	FVector getRangedAttackLocation(FVector myPosition, AActor *requester);

	UFUNCTION(BlueprintCallable)
	FVector getRangedAttackPoint(FVector attackLocation);
	
	UFUNCTION()
	void OnAttackerDeath(AActor* DestroyedActor);
private:
	//void SubscribeToWallExplosions(UObject object)
	// castle needs to listen to 7 evemt dispatchers
	bool listening[7] = { false };
	void GenerateAttackLocations();
};
