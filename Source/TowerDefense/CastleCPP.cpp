// Fill out your copyright notice in the Description page of Project Settings.

#include "CastleCPP.h"
#include "CastleWall.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values
ACastleCPP::ACastleCPP()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent *root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	for (int i = 0; i < 7; i++)
	{
		CreateWallComponent(i);
	}

}
void ACastleCPP::CreateWallComponent(int index)
{
	FString name = FString::Printf(TEXT("WallComponent_%d"), index);
	UChildActorComponent *wallComponent = CreateDefaultSubobject<UChildActorComponent>(*name);
	wallComponent->AttachTo(RootComponent);
	wallComponent->SetChildActorClass(ACastleWall::StaticClass());

	Walls.Add(wallComponent);
}

void ACastleCPP::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	for (int i = 0; i < 7; i++)
	{
		Walls[i]->CreateChildActor();
		if (Walls[i]->GetChildActor() == NULL)
			UE_LOG(LogTemp, Warning, TEXT("WALL ACTOR INVALID DURING CONSTRUCTION OF CASTLE"))
		else
		{
			ACastleWall *wall = Cast<ACastleWall>(Walls[i]->GetChildActor());
			if (wall->IsValidLowLevel())
			{
				wall->wallID = i + 1;
				FVector origin;
				FVector boxExtent;

				if (healthBarClass.Get()->IsValidLowLevel())
				{
					wall->SetWidgetClass(&healthBarClass);
				}
				wall->UpdateMeshes();
				wall->SetHealthbarHeightOffset(healthBarHeightOffset);
			}
		}
	}

}
FVector ACastleCPP::getRangedAttackLocation(FVector myPosition, AActor *requester)
{
	// free up attack location on requester death
	FVector closest = FVector::ZeroVector;
	FVector requesterLocation = requester->GetActorLocation();
	float shortestDistance = 9999999999.f;
	int locationIndex = 0;

	for (int i = 0; i < AttackLocations.Num(); i++)
	{
		FAttackLocation *a = AttackLocations[i];
		if (a->available)
		{
			float distance = FVector::Distance(a->location, requesterLocation);
			if (distance < shortestDistance)
			{

				shortestDistance = distance;
				closest = a->location;
				locationIndex = i;
			}
		}
	}
	AttackLocations[locationIndex]->available = false;
	requester->OnDestroyed.AddDynamic(this, &ACastleCPP::OnAttackerDeath);
	return closest;
}
FVector ACastleCPP::getRangedAttackPoint(FVector attackLocation)
{
	float closestDistance = 99999999.9f;
	FVector closestAttackPoint = FVector::ZeroVector;
	for (UChildActorComponent *c : Walls)
	{


		ACastleWall *w = Cast<ACastleWall>(c->GetChildActor());
		FVector wallCenter = w->GetCenter();

		float distance = FVector::Distance(attackLocation, wallCenter);
		if (distance < closestDistance)
		{
			closestAttackPoint = wallCenter;
			closestDistance = distance;
		}
	}
	return closestAttackPoint;
}

void ACastleCPP::WallExploded(int WallID, float HealthPercentage)
{
	UE_LOG(LogTemp, Warning, TEXT("Wall %d exploded, health left: %f"), WallID, HealthPercentage)
	onWallExplode.Broadcast(WallID, HealthPercentage);
}

void ACastleCPP::BeginPlay()
{
	// listen to wall delegates
	Super::BeginPlay();
	for (int i = 0; i < 7; i++)
	{
		if (!Walls[i]->GetChildActor()->IsValidLowLevel())
			UE_LOG(LogTemp, Warning, TEXT("WALL ACTOR NOT SPAWNED DURING BEGIN PLAY"))
		else
		{
			ACastleWall *wall = Cast<ACastleWall>(Walls[i]->GetChildActor());
			if (!listening[i])
			{
				wall->onWallExplode.AddDynamic(this, &ACastleCPP::WallExploded);
				listening[i] = false;
			}
		}
	}
	GenerateAttackLocations();
}
void ACastleCPP::GenerateAttackLocations()
{
	float rotationStep = (360.0f / (float)AttackLocationCount)* 3.14159265359 / 180.0f;
	FVector CastleCenter = GetActorLocation();
	
	for (int i = 0; i < AttackLocationCount; i++)
	{
		FVector attackLocation = FVector(AttackLocationDistance*FMath::Sin(rotationStep*i),
			AttackLocationDistance*FMath::Cos(rotationStep*i),
			0);
		attackLocation += GetActorLocation();

		FAttackLocation *loc = new FAttackLocation();
		loc->available = true;
		loc->location = attackLocation;
		AttackLocations.Add(loc);
		
		if (DebugActorClass->IsValidLowLevel())
		GetWorld()->SpawnActor(DebugActorClass, &attackLocation, &FRotator::ZeroRotator);
	}
}
void ACastleCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACastleCPP::OnAttackerDeath(AActor* DestroyedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("CASTLE KNOW THAT ACTOR %s WAS DESTROYED"), *DestroyedActor->GetName())
}


