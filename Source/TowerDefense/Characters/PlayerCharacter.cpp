// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "CastleCPP.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> castles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACastleCPP::StaticClass(), castles);
	if (castles.Num() > 0)
	{
		ACastleCPP* playerCastle = Cast<ACastleCPP>(castles[0]);
		if (playerCastle->IsValidLowLevel())
		{
			playerCastle->onWallExplode.AddDynamic(this, &APlayerCharacter::OnCastleWallExploded);
		}
	}
}
void APlayerCharacter::OnCastleWallExploded(int WallID, float HealthPercentage)
{
	UE_LOG(LogTemp, Warning, TEXT("Player knows that wall has exploded"));
}
// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::lookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::turn);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::moveRight);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::jump);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void APlayerCharacter::lookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}
void APlayerCharacter::turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}
void APlayerCharacter::moveForward(float AxisValue)
{
	FVector forward = GetActorRotation().Quaternion().GetForwardVector();
	AddMovementInput(forward, AxisValue);
}
void APlayerCharacter::jump()
{
	Jump();
}

void APlayerCharacter::moveRight(float AxisValue)
{
	FVector right = GetActorRotation().Quaternion().GetRightVector();
	AddMovementInput(right, AxisValue);
}
void APlayerCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	FString name = NewController->GetName();
}