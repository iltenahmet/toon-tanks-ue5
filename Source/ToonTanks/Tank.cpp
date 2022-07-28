// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComp -> SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp -> SetupAttachment(SpringArmComp);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
        RotateTurret(HitResult.ImpactPoint);
        RotateTank(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &ATank::MoveSideways);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::MoveForward(float MoveForwardValue)
{
    FVector DeltaLocation(0);
    DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);

    DeltaLocation.X = MoveForwardValue * DeltaSeconds * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::MoveSideways(float MoveSidewaysValue)
{
    FVector DeltaLocation(0);
    DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);

    DeltaLocation.Y = MoveSidewaysValue * DeltaSeconds * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::RotateTank(FVector LookAtTarget)
{
    FVector ToTarget = LookAtTarget - RootComponent->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	RootComponent->SetWorldRotation
    (
		FMath::RInterpTo
        (
			RootComponent->GetComponentRotation(), 
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			TurnSpeed
		)
	);
}
