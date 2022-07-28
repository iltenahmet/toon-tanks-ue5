// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const {return TankPlayerController;}; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComp; 

	void MoveForward(float MoveForwardValue);
	void MoveSideways(float MoveSidewaysValue);

	float DeltaSeconds;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 500;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 2.f;


	void RotateTank(FVector LookAtTarget);
	
	APlayerController* TankPlayerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
