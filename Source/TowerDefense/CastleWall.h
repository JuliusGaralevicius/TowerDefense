// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageReceiverCPP.h"
#include "StaticLibrary.h"
#include "CastleWall.generated.h"

class UDestructibleComponent;
class UStaticMeshComponent;
class UHealthBarCPP;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWallExplode, int32, WallID, float, HealthPercentage);
	
UCLASS()
class TOWERDEFENSE_API ACastleWall : public AActor, public IDamageReceiverCPP
{

	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ACastleWall(const FObjectInitializer& ObjectInitializer);

	// Maximum Health of a wall
	UPROPERTY(EditAnywhere)
	float maxHealth = 2000.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "7"))
	int wallID = 1;

	UPROPERTY(BlueprintAssignable)
	FOnWallExplode onWallExplode;
	
protected:
	// Called when the game starts or when spawned


public:	
	// Called every frame
	FVector GetCenter();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform & Transform) override;
	void SetWidgetClass(TSubclassOf<UHealthBarCPP> *widgetClass);
	void SetHealthbarHeightOffset(float height);
	void UpdateMeshes();

	bool OnDamageReceived_Implementation(float damage, FVector ImpactPoint) override;


	UFUNCTION()
	void OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



private:
	virtual void BeginPlay() override;

	float health;

	TArray<UStaticMeshComponent*> walls;
	TArray<UDestructibleComponent*> destructibles;
	UWidgetComponent *healthBarComponent = NULL;

	TArray<FString> destructiblePostfixes;
	TArray<FString> staticMeshPostfixes;

	UHealthBarCPP *healthBar = NULL;

	FString path = "/Game/Environment/Castle/Castle_destructible/";
	FVector Center;


	bool bWallExploded[3] = { false, false, false };

	void CreateDestructibleComponent(int index);
	void CreateStaticMeshComponent(int index);
	void SetPostfixes();
	void Explode(int index, FVector impactPoint);
};
