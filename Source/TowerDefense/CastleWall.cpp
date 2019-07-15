// Fill out your copyright notice in the Description page of Project Settings.

#include "CastleWall.h"
#include "DestructibleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HealthBarCPP.h"
#include "ConstructorHelpers.h"

// Sets default values
ACastleWall::ACastleWall(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetPostfixes();

	USceneComponent *root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	root->SetMobility(EComponentMobility::Movable);
	RootComponent = root;
	
	for (int i = 0; i < 3; i++)
	{
		CreateStaticMeshComponent(i);
		CreateDestructibleComponent(i);
	}
	health = maxHealth;

	healthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WallHealthWidget"));
	healthBarComponent->AttachTo(GetRootComponent());
}

// Called when the game starts or when spawned
void ACastleWall::BeginPlay()
{
	Super::BeginPlay();
	healthBar = Cast<UHealthBarCPP>(healthBarComponent->GetUserWidgetObject());
}

// Called every frame
void ACastleWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACastleWall::OnDamageReceived_Implementation(float damage, FVector ImpactPoint)
{
	health -= damage;
	UE_LOG(LogTemp, Warning, TEXT("WALL RECEIVED %f DAMAGE"), damage)
	if (healthBar->IsValidLowLevel())
	{
		healthBar->OnHealthChange(maxHealth, health);
	}
	if (health <= 0)
	{
		Explode(2, ImpactPoint);
	}
	else if (health <= maxHealth / 3)
	{
		Explode(1, ImpactPoint);
	}
	else if (health <= maxHealth / 3.0f * 2.0f)
	{
		Explode(0, ImpactPoint);
	}
	return true;
}
void ACastleWall::CreateStaticMeshComponent(int index)
{
	FString staticName = FString::Printf(TEXT("StaticWall%d"), index);
	FName name = FName(*staticName);
	UStaticMeshComponent *wall = CreateDefaultSubobject<UStaticMeshComponent>(name);
	wall->AttachTo(GetRootComponent());

	wall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	wall->SetNotifyRigidBodyCollision(true);

	wall->OnComponentHit.AddDynamic(this, &ACastleWall::OnHitCallback);
	walls.Add(wall);
}
void ACastleWall::CreateDestructibleComponent(int index)
{
	FString destructibleName = FString::Printf(TEXT("Destructible%d"), index);
	FName name = FName(*destructibleName);
	UDestructibleComponent *destructible = CreateDefaultSubobject<UDestructibleComponent>(name);
	destructible->AttachTo(GetRootComponent());

	destructible->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	destructible->SetNotifyRigidBodyCollision(false);
	destructible->SetVisibility(false, true);

	destructibles.Add(destructible);
}
void ACastleWall::SetPostfixes()
{
	destructiblePostfixes.Add(FString("_u_DM"));
	destructiblePostfixes.Add(FString("_m_DM"));
	destructiblePostfixes.Add(FString("_b_DM"));


	staticMeshPostfixes.Add(FString("_u"));
	staticMeshPostfixes.Add(FString("_m"));
	staticMeshPostfixes.Add(FString("_b"));
}
void ACastleWall::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	// this is not neccessary, only for demonstration purposes
	UpdateMeshes();
}

void ACastleWall::OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Do something when wall is hit
}
void ACastleWall::Explode(int index, FVector impactPoint)
{
	if (!bWallExploded[index])
	{
		walls[index]->SetStaticMesh(NULL);

		destructibles[index]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		destructibles[index]->SetNotifyRigidBodyCollision(true);
		destructibles[index]->SetVisibility(true, true);
		destructibles[index]->ApplyRadiusDamage(1000.0f, impactPoint, 20000.0f, 1000.0f, true);

		onWallExplode.Broadcast(wallID, health / maxHealth);

		bWallExploded[index] = true;
	}
}
void ACastleWall::UpdateMeshes()
{
	for (int i = 0; i < 3; i++)
	{

		FString postfixstatic = staticMeshPostfixes[i];
		FString staticMeshPath = FString::Printf(TEXT("%s0%d%s.0%d%s"), *path, wallID, *postfixstatic, wallID, *postfixstatic);
		UStaticMesh *stat = LoadObject<UStaticMesh>(NULL, *staticMeshPath);

		if (stat == NULL)
			UE_LOG(LogTemp, Warning, TEXT("COULD NOT FIND ASSET %s"), *staticMeshPath)
		else
			walls[i]->SetStaticMesh(stat);


		FString postfixdestr = destructiblePostfixes[i];
		FString destrMeshPath = FString::Printf(TEXT("%s0%d%s.0%d%s"), *path, wallID, *postfixdestr, wallID, *postfixdestr);
		UDestructibleMesh *destr = LoadObject<UDestructibleMesh>(NULL, *destrMeshPath);

		if (destr == NULL)
			UE_LOG(LogTemp, Warning, TEXT("COULT NOT FIND ASSET %s"), *destrMeshPath)
		else
			destructibles[i]->SetDestructibleMesh(destr);
	}
}
FVector ACastleWall::GetCenter()
{
	return walls[1]->Bounds.Origin;
}
void ACastleWall::SetWidgetClass(TSubclassOf<UHealthBarCPP> *widgetClass)
{
	healthBarComponent->SetWidgetClass(widgetClass->Get());
}
void ACastleWall::SetHealthbarHeightOffset(float height)
{
	healthBarComponent->SetWorldLocation(GetCenter() + FVector(0, 0, height));
}
